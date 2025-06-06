#include "str.hpp"
#include "../test/testing.hpp"

#include <cassert>
#include <filesystem>
#include <iostream>
#include <variant>
#include <vector>

//! 节点类型表
///
/// 用法：DEF_NODEKIND(Priority_, Name_, Type_, Desc_)
#define NODEKIND_TABLE()                 \
    DEF_NODEKIND(00, NEOF, NONE, "文档结束")    \
    /* 容器 */                           \
    DEF_NODEKIND(10, DOCUMENT, node_document, "文档根") \
    DEF_NODEKIND(11, BLOCK, node_block, "块")        \
    DEF_NODEKIND(12, LINE, node_line, "行")         \
    /* 独立行 */                         \
    DEF_NODEKIND(20, TITLE, node_title, "文档标题")  \
    DEF_NODEKIND(21, H1, node_head, "标题1")        \
    DEF_NODEKIND(22, H2, node_head, "标题2")        \
    DEF_NODEKIND(23, H3, node_head, "标题3")        \
    DEF_NODEKIND(24, H4, node_head, "标题4")        \
    DEF_NODEKIND(25, H5, node_head, "标题5")        \
    DEF_NODEKIND(26, H6, node_head, "标题6")        \
    /* 独立行 */                         \
    DEF_NODEKIND(31, S1, node_section, "段1")          \
    DEF_NODEKIND(32, S2, node_section, "段2")          \
    DEF_NODEKIND(33, S3, node_section, "段3")          \
    DEF_NODEKIND(34, S4, node_section, "段4")          \
    /* 行间 */                           \
    DEF_NODEKIND(50, COMMENT, node_comment, "注释")    \
    DEF_NODEKIND(52, THEAD, node_thead, "表头")      \
    DEF_NODEKIND(53, TROW, node_trow, "表的行")     \
    DEF_NODEKIND(54, TCOL, node_tcol, "表的列")     \
    DEF_NODEKIND(55, PARAM, node_param, "参数定义")  \
    DEF_NODEKIND(56, RETURN, node_return, "返回值")   \
    DEF_NODEKIND(57, BCODE, node_bcode, "代码块")    \
    DEF_NODEKIND(58, BRIEF, node_brief, "概要")      \
    DEF_NODEKIND(59, DETIAL, node_detial, "详情")     \
    DEF_NODEKIND(60, FORMULA, node_formula, "公式")    \
    DEF_NODEKIND(63, OL, node_orderd_list, "有序列表")     \
    DEF_NODEKIND(64, UL, node_unorderd_list, "无序列表")     \
    DEF_NODEKIND(65, LI, node_list_item, "列表子项")     \
    DEF_NODEKIND(66, HREF, node_href, "超链接")     \
    DEF_NODEKIND(67, IMAGE, node_image, "图片")      \
    DEF_NODEKIND(68, ANCHOR, node_anchor, "锚点定义") \
    DEF_NODEKIND(69, EMBDED, node_embded, "嵌入文字") \
    /* 行内元素 */                       \
    DEF_NODEKIND(80, STRONG, node_strong, "强调")     \
    DEF_NODEKIND(81, EM, node_em, "重要")         \
    DEF_NODEKIND(82, TEXT, node_text, "文本")       \
    DEF_NODEKIND(83, ICODE, node_icode, "行内代码")  \
/* (end) */

enum node_kind : uint8_t {
#define DEF_NODEKIND(Priority_, Name_, Type_, Desc_) NODE_KIND_##Name_ = (Priority_),
    NODEKIND_TABLE()
#undef DEF_NODEKIND
};

enum node_priority : uint8_t {
#define DEF_NODEKIND(Priority_, Name_, Type_, Desc_) NODE_PRIORITY_##Name_ = (Priority_),
    NODEKIND_TABLE()
#undef DEF_NODEKIND
};

//! 标记位
enum node_flags : uint32_t {
    FLAG_INLINE = 0x0001,     ///< 是否行内节点
    FLAG_BLOCK = 0x0002,      ///< 是否块级节点
    FLAG_STANDALONE = 0x0004, ///< 独占一行
};

struct list_head {
    list_head* next;
    list_head* prev;
};

static inline auto list_head_init(list_head* head) -> list_head* {
    head->next = head;
    head->prev = head;
    return head;
}

static inline auto list_insert(list_head* prev, list_head* next, list_head* new_item) -> void {
    assert(prev != nullptr);
    assert(next != nullptr);
    assert(new_item != nullptr);
    new_item->next = next;
    new_item->prev = prev;
    prev->next = new_item;
    next->prev = new_item;
}

static inline auto list_append(list_head* head, list_head* new_item) -> void {
    assert(head != nullptr);
    assert(new_item != nullptr);
    list_insert(head->prev, head, new_item);
}

static auto list_prepend(list_head* head, list_head* new_item) -> void {
    assert(new_item != nullptr);
    list_insert(head, head->next, new_item);
}

static auto list_split(list_head* old_item) -> void {
    assert(old_item != nullptr);
    old_item->prev->next = old_item->next;
    old_item->next->prev = old_item->prev;
    list_head_init(old_item);
}

struct node {
    // 所有可能的子节点
    list_head children{&children, &children};

    // 父节点
    node* parent{nullptr};


    // 节点类型
    node_kind kind{NODE_KIND_TEXT};

    // 节点标记位
    uint32_t flags{0}; // node_flags

    // 符号优先级
    int8_t priority{0};

    auto append(node* child) -> void {
        assert(child != nullptr);
        list_append(&children, reinterpret_cast<list_head*>(child));
        child->parent = this;
    }

    auto prepend(node* child) -> void {
        assert(child != nullptr);
        list_prepend(&children, reinterpret_cast<list_head*>(child));
        child->parent = this;
    }

    auto split() -> node* {
        list_split(reinterpret_cast<list_head*>(this));
        return this;
    }
};

struct node_document : public node {
    explicit node_document() {
        kind = NODE_KIND_DOCUMENT;
        priority = NODE_PRIORITY_DOCUMENT;
    }
};

struct node_block : public node {
    explicit node_block() {
        kind = NODE_KIND_BLOCK;
    }
};

struct node_line : public node {
    int32_t line_no{};

    explicit node_line() {
        kind = NODE_KIND_LINE;
    }
};

struct node_title : public node {
    explicit node_title() {
        kind = NODE_KIND_TITLE;
    }
};

struct node_head : public node {
    explicit node_head(int8_t level) {
        assert((level >= 1) && (level <= 6));
        kind = static_cast<node_kind>(NODE_KIND_H1 + (level - 1));
    }
};

struct node_section : public node {
    explicit node_section(int8_t level) {
        assert((level >= 1) && (level <= 4));
        kind = static_cast<node_kind>(NODE_KIND_S1 + (level - 1));
    }
};

struct node_comment : public node {
    std::vector<std::string> lines;

    explicit node_comment() {
        kind = NODE_KIND_COMMENT;
    }
};

struct node_bcode : public node {
    std::vector<std::string> lines;

    explicit node_bcode() {
        kind = NODE_KIND_BCODE;
    }
};

struct node_icode : public node {
    std::string text;

    explicit node_icode() {
        kind = NODE_KIND_ICODE;
        flags |= node_flags::FLAG_INLINE;
    }
};

struct node_embded : public node {
    std::string file_name;
    std::string block_name;

    explicit node_embded() {
        kind = NODE_KIND_EMBDED;
    }
};

struct node_unorderd_list : public node {
    int32_t level{};

    explicit node_unorderd_list(int32_t l) {
        kind = NODE_KIND_UL;
        level = l;
    }
};

struct node_href : public node {
    std::string name;
    std::string url;

    explicit node_href() {
        kind = NODE_KIND_HREF;
    }
};

struct node_image : public node {
    std::string name;
    std::string url;

    node_image() {
        kind = NODE_KIND_IMAGE;
    }
};

struct node_anchor : public node {
    std::vector<std::string> names;

    explicit node_anchor() {
        kind = NODE_KIND_ANCHOR;
    }
};

struct node_param : public node {
    std::vector<std::string> names;

    explicit node_param() {
        kind = NODE_KIND_PARAM;
    }
};

struct node_return : public node {
    explicit node_return() {
        kind = NODE_KIND_RETURN;
    }
};

struct will {
    template <typename KeyType>
    class key {
    public:
        explicit key(const KeyType& k)
            : key_{k} {
        }

        template <typename SetType>
        inline auto in(const SetType& sets) const -> bool {
            return sets.find(key_) != sets.cend();
        }

        template <typename SetType>
        inline auto notin(const SetType& sets) const -> bool {
            return sets.find(key_) == sets.cend();
        }

    private:
        const KeyType& key_;
    };

    template <typename IndexType>
    class pos {
    public:
        explicit pos(const IndexType& k)
            : pos_{k} {
        }

        template <typename SequenceType>
        inline auto in(const SequenceType& seqs) const -> bool {
            return (pos_ >= 0) && (pos_ < seqs.size());
        }

        template <typename SetType>
        inline auto notin(const SetType& sets) const -> bool {
            return !in(sets);
        }

    private:
        const IndexType& pos_;
    };
};

auto cmd_help(int argc, char* artgv[]) -> int {
    constexpr std::string_view help_text = //
        "Usage:\n"                         //
        "   gendoc [InputFiles...] [-o OutputDirectory] [-r RootDirectory]\n";
    std::cout << help_text;

    return 0;
}

struct gendoc_options {
    std::vector<std::string> input_files;
    std::string output_directory;
    std::string root_directory;

    auto load(int argc, char* argv[]) -> std::string {
        int pos = 1;

        std::optional<str::pair<std::string_view>> keyval;
        while (keyval = str::next_opt2(pos, argc, argv)) {
            std::string_view key = std::get<0>(*keyval);
            std::string_view val = std::get<1>(*keyval);
            key = str::trim_surrounding_view(key);
            val = str::trim_surrounding_view(val);
            if (key.empty() && val.empty()) {
                continue;
            }

            if (key == "-o") {
                output_directory = val;
                continue;
            }

            if (key == "-r") {
                root_directory = val;
                continue;
            }

            input_files.emplace_back(val);
        }

        if (output_directory.empty()) {
            output_directory = std::filesystem::current_path().string();
        }

        if (root_directory.empty()) {
            root_directory = std::filesystem::current_path().string();
        }

        return {};
    }
};

enum class parse_state : uint16_t {
    WAIT_BLOCK,
};

struct line_type {
    size_t line_no{0};
    std::string line_text;

    explicit line_type() = default;

    explicit line_type(size_t no, std::string_view text) {
        line_no = no;
        line_text = text;
    }
};

class line_reader {
public:
    explicit line_reader(FILE* file) {
        assert(file != nullptr);
        file_ = file;
    }

    inline auto line_no() const -> size_t {
        return line_.line_no;
    }

    inline auto line_text() const -> const std::string& {
        return line_.line_text;
    }

    inline auto line() const -> const line_type& {
        return line_;
    }

    inline auto next_line() -> bool {
        if (!cached_lines_.empty()) {
            line_ = std::move(cached_lines_.back());
            cached_lines_.pop_back();
            return true;
        }

        auto result = str::read_next_line(file_, false);
        if (!result) {
            return false;
        }

        line_.line_no++;
        line_.line_text = std::move(result.value());

        return true;
    }

    inline auto rollback(size_t line_no, std::string_view line_text) -> void {
        cached_lines_.emplace_back(line_no, line_text);
    }

    inline operator bool() const {
        return !feof(file_) && !ferror(file_);
    }

private:
    mutable FILE* file_{nullptr};
    line_type line_;
    std::vector<line_type> cached_lines_;
};

struct render_context {
    explicit render_context(FILE* f, const std::string& od, const std::string& rd)
        : output_directory{od}
        , root_directory{rd}
        , reader{f} {
        root = new node;
        parent = root;
    }

    const std::string& output_directory;
    const std::string& root_directory;
    line_reader reader;

    node* root{nullptr};
    node* parent{nullptr};

    // 优先级队列
    std::vector<node*> stack;
};

auto try_parse_line(render_context& context) -> void;

auto try_parse_head(render_context& context) -> void {
    static std::regex hx_pattern{R"(^(#+)\s+(.*))"};
    const auto& line = context.reader.line_text();
    std::smatch match;
    std::regex_match(line, match, hx_pattern);
    int32_t level = static_cast<int32_t>(match[1].length());

    node_head* head = new node_head(static_cast<int8_t>(level));
    context.parent->append(head);
    context.parent = head;
    try_parse_line(context);
    context.parent = context.parent->parent;
}

// % xxx
auto try_parse_section(render_context& context) -> void {
    static std::regex hx_pattern{R"(^(%+)\s+(.*))"};
    const auto& line = context.reader.line_text();
    std::smatch match;
    std::regex_match(line, match, hx_pattern);
    int32_t level = static_cast<int32_t>(match[1].length());

    node_section* section = new node_section(static_cast<int8_t>(level));
    context.parent->append(section);
    context.parent = section;
    try_parse_line(context);
    context.parent = context.parent->parent;
}

// * xxx
auto try_parse_unordered_list(render_context& context) -> void {
    auto& line = context.reader.line_text();
    str::size_type pos = 0;
    auto spaces = str::next_spaces_range(line, pos);
    int32_t level = static_cast<int32_t>(spaces->size() / 2);
    node_unorderd_list* ul = new node_unorderd_list(level);

    if (context.parent->kind != NODE_KIND_UL) {
        context.parent->append(ul);
        context.parent = ul;
        try_parse_line(context);
        context.parent = context.parent->parent;
        return;
    }


}

auto try_parse_line(render_context& context) -> void {
}

auto try_accept_block_text(render_context& context) -> void {
    node_block* block = new node_block();
    context.parent->append(block);
    context.parent = block;

    do {
        auto& line = context.reader.line_text();

        // 连续的文本行遇到了空行或者空白行，字节跳过
        if (str::is_space_or_empty(line)) {
            break;
        }

        node_line* nline = new node_line;
        context.parent->append(nline);
        context.parent = nline;
        try_parse_line(context);
        context.parent = nline->parent;
    } while (context.reader.next_line()) ;

    context.parent = context.parent->parent;
}

auto try_parse_comment(render_context& context) -> void {
    node_comment* comment = new node_comment;
    context.parent->append(comment);

    do {
        comment->lines.emplace_back(context.reader.line_text());
        if (str::ends_with(context.reader.line_text(), "-->")) {
            return;
        }
    } while (context.reader.next_line());
}

auto try_parse_param(render_context& context) -> void {
    static std::regex param_pattern1{R"(^\s*@param\s+([a-zA-Z_][0-9a-zA-Z_]*)(,\s+([a-zA-Z_][0-9a-zA-Z_]*))?:(.*))"};
    static std::regex param_pattern2{R"(^\s*@param\{([a-zA-Z_][0-9a-zA-Z_]*)(,\s+[a-zA-Z_][0-9a-zA-Z_]*)?\}(.*))"};

    std::smatch match;
    if (std::regex_match(context.reader.line_text(), match, param_pattern1)) {

    } else if (std::regex_match(context.reader.line_text(), match, param_pattern2)) {

    }

}

auto try_parse_return(render_context& context) -> void {
    static std::regex return_pattern{R"(^\s*@return:\s+(.*))"};
    node_return* nret = new node_return;
    context.parent->append(nret);
}

auto try_parse_bcode(render_context& context) -> void {
    node_bcode* bcode = new node_bcode;
    context.parent->append(bcode);
    while (context.reader.next_line()) {
        if (str::starts_with(context.reader.line_text(), "```")) {
            break;
        }

        bcode->lines.emplace_back(context.reader.line_text());
    }
}

auto try_parse_block(render_context& context) -> void {
    parse_state state = parse_state::WAIT_BLOCK;
    while (context.reader.next_line()) {
        std::string_view line = context.reader.line_text();

        if (line.empty()) {
            // if (context.parent->flags & node_flags::FLAG_STANDALONE) {
            //     context.parent = context.parent->parent;
            //     continue;
            // }
            continue;
        }

        // @param 开头的行
        if (str::starts_with_word(line, "@param")) {
            try_parse_param(context);
        }

        // @return 开头的行
        if (str::starts_with_word(line, "@return")) {
            try_parse_return(context);
            continue;
        }

        // 代码块
        if (str::starts_with_word(line, "```")) {
            try_parse_bcode(context);
            continue;
        }

        //! 注释块: <!-- ... -->
        if (str::starts_with_word(line, "<!--")) {
            try_parse_comment(context);
            continue;
        }

        // 标题行：# 号开头的行
        static std::regex hx_pattern{R"(^#+\s.*)"};
        if (std::regex_match(line.begin(), line.end(), hx_pattern)) {
            try_parse_head(context);
            continue;
        }

        // 章节内分级： % 号开头的行
        static std::regex gx_pattern{R"(^%(#)\s.*)"};
        if (std::regex_match(line.begin(), line.end(), hx_pattern)) {
            try_parse_head(context);
            continue;
        }

        // 无需列表行
        static std::regex ul_pattern{R"(\*+\s.*)"};
        if (std::regex_match(line.begin(), line.end(), ul_pattern)) {
            try_parse_unordered_list(context);
            continue;
        }

        try_accept_block_text(context);
    }
}

auto try_parse_document(render_context& context) -> void {
    try_parse_block(context);
}

auto render_one_file(render_context& context) -> std::string {
    if (context.reader) {
        try_parse_document(context);
        return "";
    }

    return "error-1";
}

auto main(int argc, char* argv[]) -> int {
    gendoc_options opts;
    std::string error = opts.load(argc, argv);
    if (!error.empty()) {
        std::cerr << error << std::endl;
        return 1;
    }

    if (!std::filesystem::exists(opts.output_directory)) {
        std::error_code error;
        if (!std::filesystem::create_directories(opts.output_directory, error)) {
            std::cerr << "Can not create output directory `" << opts.output_directory << "'" << std::endl;
            return 1;
        }
    }

    if (!std::filesystem::exists(opts.root_directory)) {
        std::cerr << "The project root directory is not exist `" << opts.root_directory << "'" << std::endl;
        return 1;
    }

    if (opts.input_files.empty()) {
        render_context context{stdin, opts.output_directory, opts.root_directory};
        std::string error = render_one_file(context);
        if (!error.empty()) {
            std::cerr << "Generate document failed: " << error << std::endl;
            return 1;
        }

        return 0;
    }

    for (const auto& filepath : opts.input_files) {
        std::string error;
        str::with_file(filepath, "r", [&opts, &error](FILE* file) -> void {
            assert(file != nullptr);
            render_context context{file, opts.output_directory, opts.root_directory};
            error = render_one_file(context);
        });

        if (!error.empty()) {
            std::cerr << "Generate document failed: " << error << std::endl;
            return 1;
        }
    }

    return 0;
}
