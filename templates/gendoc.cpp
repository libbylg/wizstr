#include "str.hpp"

#include <cassert>
#include <filesystem>
#include <iostream>
#include <tuple>
#include <tuple>
#include <variant>
#include <vector>

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

//! 节点类型表
///
/// 用法：DEF_NODEKIND(Priority_, Name_, Type_, Desc_)
#define NODEKIND_TABLE()                                 \
    DEF_NODEKIND(00, NEOF, NONE, "文档结束")             \
    DEF_NODEKIND(01, GUARD, NONE, "节点守卫")            \
    /* 容器 */                                           \
    DEF_NODEKIND(10, DOCUMENT, node_document, "文档根")  \
    DEF_NODEKIND(11, BLOCK, node_block, "块")            \
    DEF_NODEKIND(12, LINE, node_line, "行")              \
    /* 独立行 */                                         \
    DEF_NODEKIND(20, TITLE, node_title, "文档标题")      \
    DEF_NODEKIND(21, HEAD, node_head, "标题")            \
    /* 独立行 */                                         \
    DEF_NODEKIND(31, SECTION, node_section, "段")        \
    /* 行间 */                                           \
    DEF_NODEKIND(50, COMMENT, node_comment, "注释")      \
    DEF_NODEKIND(52, THEAD, node_thead, "表头")          \
    DEF_NODEKIND(53, TROW, node_trow, "表的行")          \
    DEF_NODEKIND(54, TCOL, node_tcol, "表的列")          \
    DEF_NODEKIND(55, PARAM, node_param, "参数定义")      \
    DEF_NODEKIND(56, RETURN, node_return, "返回值")      \
    DEF_NODEKIND(57, BCODE, node_bcode, "代码块")        \
    DEF_NODEKIND(58, BRIEF, node_brief, "概要")          \
    DEF_NODEKIND(59, DETIAL, node_detial, "详情")        \
    DEF_NODEKIND(60, IFORMULA, node_iformula, "行内公式")\
    DEF_NODEKIND(61, BFORMULA, node_bformula, "行间公式")\
    DEF_NODEKIND(63, OL, node_orderd_list, "有序列表")   \
    DEF_NODEKIND(64, UL, node_unorderd_list, "无序列表") \
    DEF_NODEKIND(65, LI, node_list_item, "列表子项")     \
    DEF_NODEKIND(66, HLINK, node_hlink, "超链接")        \
    DEF_NODEKIND(67, IMAGE, node_image, "图片")          \
    DEF_NODEKIND(68, ANCHOR, node_anchor, "锚点定义")    \
    DEF_NODEKIND(69, EMBDED, node_embded, "嵌入文字")    \
    /* 行内元素 */                                       \
    DEF_NODEKIND(80, STRONG, node_strong, "强调")        \
    DEF_NODEKIND(81, EM, node_em, "重要")                \
    DEF_NODEKIND(82, TEXT, node_text, "文本")            \
    DEF_NODEKIND(83, ICODE, node_icode, "行内代码")      \
    DEF_NODEKIND(84, ANNO, node_anno, "注解")            \
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

enum node_limits {
    HEAD_LEVEL_MIN = 1,
    HEAD_LEVEL_MAX = 6,

    SECTION_LEVEL_MIN = 1,
    SECTION_LEVEL_MAX = 4,
};

//! 标记位
enum node_flags : uint32_t {
    FLAG_INLINE = 0x0001,     ///< 是否行内节点
    FLAG_BLOCK = 0x0002,      ///< 是否块级节点
    FLAG_STANDALONE = 0x0004, ///< 独占一行
};

template <typename T>
struct list_head {
    T* next;
    T* prev;
};

template <typename H>
static inline auto list_head_init(H* head) -> H* {
    head->next = reinterpret_cast<decltype(head->next)>(head);
    head->prev = reinterpret_cast<decltype(head->next)>(head);
    return head;
}

template <typename P, typename N, typename T>
static inline auto list_insert(P* prev, N* next, T* new_item) -> void {
    assert(prev != nullptr);
    assert(next != nullptr);
    assert(new_item != nullptr);
    new_item->next = reinterpret_cast<decltype(new_item->next)>(next);
    new_item->prev = reinterpret_cast<decltype(new_item->prev)>(prev);
    prev->next = reinterpret_cast<decltype(prev->next)>(new_item);
    next->prev = reinterpret_cast<decltype(next->prev)>(new_item);
}

template <typename H, typename T>
static inline auto list_append(H* head, T* new_item) -> void {
    assert(head != nullptr);
    assert(new_item != nullptr);
    list_insert(head->prev, head, new_item);
}


template <typename H, typename T>
static auto list_prepend(H* head, T* new_item) -> void {
    assert(new_item != nullptr);
    list_insert(head, head->next, new_item);
}

template <typename T>
static auto list_split(T* old_item) -> void {
    assert(old_item != nullptr);
    old_item->prev->next = old_item->next;
    old_item->next->prev = old_item->prev;
    list_head_init(old_item);
}

struct node : public list_head<node> {
    // 父节点
    node* parent{nullptr};

    // 所有可能的子节点
    list_head<node> children;

    // 节点类型
    node_kind kind{NODE_KIND_TEXT};

    // 节点标记位
    uint32_t flags{0}; // node_flags

    // 符号优先级
    int8_t priority{0};

    explicit node() {
        list_head_init(this);
        list_head_init(&children);
    }

    auto append(node* child) -> void {
        assert(child != nullptr);
        list_append(&children, child);
        child->parent = this;
    }

    auto prepend(node* child) -> void {
        assert(child != nullptr);
        list_prepend(&children, child);
        child->parent = this;
    }

    auto split() -> node* {
        list_split(this);
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

// struct node_title : public node {
//     explicit node_title() {
//         kind = NODE_KIND_TITLE;
//     }
// };

struct node_head : public node {
    explicit node_head(int8_t l) {
        assert((l >= HEAD_LEVEL_MIN) && (l <= HEAD_LEVEL_MAX));
        kind = NODE_KIND_HEAD;
        level = l;
    }

    int8_t level{0};
};

struct node_section : public node {
    explicit node_section(int8_t l) {
        assert((l >= SECTION_LEVEL_MIN) && (l <= SECTION_LEVEL_MAX));
        kind = NODE_KIND_SECTION;
        level = l;
    }

    int8_t level{0};
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

    explicit node_icode(std::string_view content) {
        kind = NODE_KIND_ICODE;
        flags |= node_flags::FLAG_INLINE;
        text = content;
    }
};

struct node_iformula : public node {
    explicit node_iformula() {
        kind = NODE_KIND_IFORMULA;
    }

    std::string text;
};

struct node_bformula : public node {
    explicit node_bformula() {
        kind = NODE_KIND_BFORMULA;
    }

    std::vector<std::string> lines;
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


struct node_anchor : public node {
    std::vector<std::string> names;

    explicit node_anchor() {
        kind = NODE_KIND_ANCHOR;
    }
};

struct node_param : public node {
    explicit node_param() {
        kind = NODE_KIND_PARAM;
    }
};

struct node_return : public node {
    explicit node_return() {
        kind = NODE_KIND_RETURN;
    }
};

struct node_text : public node {
    std::string text;

    explicit node_text(std::string_view content) {
        kind = NODE_KIND_TEXT;
        text = content;
    }
};

struct node_strong : public node {
    std::string text;

    explicit node_strong(std::string_view content) {
        kind = NODE_KIND_STRONG;
        text = content;
    }
};

struct node_em : public node {
    std::string text;

    explicit node_em(std::string_view content) {
        kind = NODE_KIND_EM;
        text = content;
    }
};

struct node_hlink : public node {
    std::string name;
    std::string url;

    explicit node_hlink(std::string_view n, std::string_view u) {
        kind = NODE_KIND_HLINK;
        name = n;
        url = u;
    }
};

struct node_image : public node {
    std::string name;
    std::string url;

    explicit node_image(std::string_view n, std::string_view u) {
        kind = NODE_KIND_IMAGE;
        name = n;
        url = u;
    }
};

struct node_anno : public node {
    std::string type;
    std::vector<std::string> names;

    explicit node_anno(std::string_view n, const std::vector<std::string_view>& u) {
        kind = NODE_KIND_ANNO;
        type = n;
        for (auto item : u) {
            names.emplace_back(item);
        }
    }
};

auto print_md(node* nd, const std::function<void(std::string_view)> print) -> void {
    assert((nd != nullptr) && (print != nullptr));
    switch (nd->kind) {
        case NODE_KIND_DOCUMENT: {
            node_document* document = reinterpret_cast<node_document*>(nd);
            for (auto child = document->children.next; child != reinterpret_cast<node*>(&document->children); child = child->next) {
                print_md(child, print);
            }
        }
        break;
        case NODE_KIND_BLOCK: {
            node_block* block = reinterpret_cast<node_block*>(nd);
            for (auto child = block->children.next; child != reinterpret_cast<node*>(&block->children); child = child->next) {
                print_md(child, print);
            }
        }
        break;
        case NODE_KIND_LINE: {
            node_block* block = reinterpret_cast<node_block*>(nd);
            for (auto child = block->children.next; child != reinterpret_cast<node*>(&block->children); child = child->next) {
                print_md(child, print);
            }
            print("\n");
        }
        break;
        // case NODE_KIND_TITLE: {
        //     node_title* title = reinterpret_cast<node_title*>(nd);
        // }
        // break;
        case NODE_KIND_HEAD: {
            node_head* head = reinterpret_cast<node_head*>(nd);
            print(str::repeat("*", head->level));
            print(" ");
            for (auto child = head->children.next; child != reinterpret_cast<node*>(&head->children); child = child->next) {
                print_md(child, print);
            }
            print("\n");
        }
        break;
        case NODE_KIND_SECTION: {
            node_section* section = reinterpret_cast<node_section*>(nd);
            auto tag = str::repeat("%", section->level);
            print(tag);
            print(" ");
            for (auto child = section->children.next; child != reinterpret_cast<node*>(&section->children); child = child->next) {
                print_md(child, print);
            }
            print(tag);
            print("<");
            print("\n");
        }
        break;
        case NODE_KIND_COMMENT: {
            node_comment* comment = reinterpret_cast<node_comment*>(nd);
            print("<!-- \n");
            for (auto& line : comment->lines) {
                print(line);
                print("\n");
            }
            print("-->\n");
        }
        break;
        case NODE_KIND_THEAD: {
        }
        break;
        case NODE_KIND_TROW: {

        }
        break;
        case NODE_KIND_TCOL: {

        }
        break;
        case NODE_KIND_PARAM: {
            node_param* param = reinterpret_cast<node_param*>(nd);
            for (auto child = param->children.next; child != reinterpret_cast<node*>(&param->children); child = child->next) {
                print_md(child, print);
            }
            print("\n");
        }
        break;
        case NODE_KIND_RETURN: {
            node_return* ret = reinterpret_cast<node_return*>(nd);
            for (auto child = ret->children.next; child != reinterpret_cast<node*>(&ret->children); child = child->next) {
                print_md(child, print);
            }
            print("\n");
        }
        break;
        case NODE_KIND_BCODE: {
            node_bcode* bcode = reinterpret_cast<node_bcode*>(nd);
            print("```\n");
            for (auto& line : bcode->lines) {
                print(line);
                print("\n");
            }
            print("```\n");
        }
        break;
        case NODE_KIND_BRIEF: {

        }
        break;
        case NODE_KIND_DETIAL: {

        }
        break;
        case NODE_KIND_IFORMULA: {
            node_iformula* iformula = reinterpret_cast<node_iformula*>(nd);
            print("$");
            print(iformula->text);
            print("$");
        }
        break;
        case NODE_KIND_BFORMULA: {
            node_bformula* bformula = reinterpret_cast<node_bformula*>(nd);
            print("$$\n");
            for (auto& line : bformula->lines) {
                print(line);
                print("\n");
            }
            print("$$\n");
        }
        break;
        case NODE_KIND_OL: {

        }
        break;
        case NODE_KIND_UL: {

        }
        break;
        case NODE_KIND_LI: {

        }
        break;
        case NODE_KIND_HLINK: {
            node_hlink* hlink = reinterpret_cast<node_hlink*>(nd);
            print("[");
            print(hlink->name);
            print("](");
            print(hlink->url);
            print(")");
        }
        break;
        case NODE_KIND_IMAGE: {
            node_image* image = reinterpret_cast<node_image*>(nd);
            print("![");
            print(image->name);
            print("](");
            print(image->url);
            print(")");
        }
        break;
        case NODE_KIND_ANCHOR: {

        }
        break;
        case NODE_KIND_EMBDED: {

        }
        break;
        case NODE_KIND_STRONG: {
            node_strong* strong = reinterpret_cast<node_strong*>(nd);
            print("**");
            print(strong->text);
            print("**");
        }
        break;
        case NODE_KIND_EM: {
            node_em* em = reinterpret_cast<node_em*>(nd);
            print("*");
            print(em->text);
            print("*");
        }
        break;
        case NODE_KIND_TEXT: {
            node_text* text = reinterpret_cast<node_text*>(nd);
            print(text->text);
        }
        break;
        case NODE_KIND_ICODE: {
            node_icode* text = reinterpret_cast<node_icode*>(nd);
            print("`");
            print(text->text);
            print("`");
        }
        break;
        case NODE_KIND_ANNO: {
            node_anno* anno = reinterpret_cast<node_anno*>(nd);
            print("@");
            print(anno->type);
            print("{");
            for (size_t index = 0; index < anno->names.size(); index++) {
                if (index != 0) [[likely]] {
                    print(", ");
                }
                print(anno->names[index]);
            }
            print("}");
        }
        break;

    }
}

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
        "   gendoc [InputFile] [-o OutputFile] [-r RootDirectory]\n";
    std::cout << help_text;

    return 0;
}

struct gendoc_options {
    std::string input_file;
    std::string output_file;
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
                output_file = val;
                continue;
            }

            if (key == "-r") {
                root_directory = val;
                continue;
            }

            input_file = val;
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
    line_type* next;
    line_type* prev;

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
    explicit render_context(FILE* ifile, FILE* ofile, const std::string& rd)
        : root_directory{rd}
        , reader{ifile}
        , writer{ofile} {
        root = new node_document;
        parent = root;
    }

    const std::string& root_directory;
    line_reader reader;
    size_t rpos{0};
    FILE* writer{nullptr};

    node* root{nullptr};
    node* parent{nullptr};

    // // 所有行
    // list_head   lines;

    // 行解析栈
    std::vector<std::string_view> line_parse_stack;

    // 优先级队列
    std::vector<node*> stack;
};


struct acceptor {
public:
    explicit acceptor(std::string_view line, str::size_type start = 0)
        : line_{line}
        , start_{start}
        , rpos_{start} {
    }

    auto from(str::size_type pos) -> acceptor& {
        rpos_ = pos;
        ok_ = true;
        return *this;
    }

    auto starts_with(std::string_view prefix) -> bool {
        return (str::starts_with(line_, rpos_, prefix));
    }

    auto accept(str::value_type expect_ch) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        auto range = str::accept(line_, rpos_, expect_ch);
        if (!range) {
            ok_ = false;
        }

        return *this;
    }

    auto accept(const str::charset_type expect_charstr) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        auto range = str::accept(line_, rpos_, expect_charstr);
        if (!range) {
            ok_ = false;
        }

        return *this;
    }

    auto accept(std::string_view expect_str) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        auto range = str::accept(line_, rpos_, expect_str);
        if (!range) {
            ok_ = false;
        }

        return *this;
    }

    auto accept(std::string_view& result, const std::regex& expect_pattern) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        auto range = str::accept(line_, rpos_, expect_pattern);
        if (!range) {
            ok_ = false;
        }

        result = str::take_view(line_, *range);
        return *this;
    }

    auto accept_until(str::value_type escape, str::value_type sep_ch) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        auto range = str::accept_until(line_, rpos_, escape, sep_ch);
        if (!range) {
            ok_ = false;
        }

        return *this;
    }

    auto accept_until(str::value_type sep_ch) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        auto range = str::accept_until(line_, rpos_, sep_ch);
        if (!range) {
            ok_ = false;
            return *this;
        }

        return *this;
    }

    auto accept_until(str::value_type escape, const str::charset_type& sep_charset) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        auto range = str::accept_until(line_, rpos_, escape, sep_charset);
        if (!range) {
            ok_ = false;
        }

        return *this;
    }

    auto accept_until(const str::charset_type& sep_charset) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        auto range = str::accept_until(line_, rpos_, sep_charset);
        if (!range) {
            ok_ = false;
            return *this;
        }

        return *this;
    }

    auto accept_until(std::string_view sep_str) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        auto range = str::accept_until(line_, rpos_, sep_str);
        if (!range) {
            ok_ = false;
        }

        return *this;
    }

    auto accept_until(const std::regex& sep_pattern) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        std::string_view remain = str::take_view(line_, rpos_);
        std::match_results<std::string_view::const_iterator> match;
        if (!std::regex_search(remain.begin(), remain.end(), match, sep_pattern)) {
            ok_ = false;
            return *this;
        }

        rpos_ = match.position(0) + match.length(0);
        return *this;
    }

    auto accept_until(std::string_view& result, str::value_type escape, str::value_type sep_ch) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        auto range = str::accept_until(line_, rpos_, escape, sep_ch);
        if (!range) {
            ok_ = false;
            return *this;
        }

        result = line_.substr(range->pos, range->len);
        return *this;
    }

    auto accept_until(std::string_view& result, str::value_type sep_ch) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        auto range = str::accept_until(line_, rpos_, sep_ch);
        if (!range) {
            ok_ = false;
            return *this;
        }

        result = line_.substr(range->pos, range->len);
        return *this;
    }

    auto accept_until(std::string_view& result, str::value_type escape, const str::charset_type& sep_charset) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        auto range = str::accept_until(line_, rpos_, escape, sep_charset);
        if (!range) {
            ok_ = false;
            return *this;
        }

        result = line_.substr(range->pos, range->len);
        return *this;
    }

    auto accept_until(std::string_view& result, const str::charset_type& sep_charset) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        auto range = str::accept_until(line_, rpos_, sep_charset);
        if (!range) {
            ok_ = false;
            return *this;
        }

        result = line_.substr(range->pos, range->len);
        return *this;
    }

    auto accept_until(std::string_view& result, std::string_view sep_str) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        auto range = str::accept_until(line_, rpos_, sep_str);
        if (!range) {
            ok_ = false;
        }

        result = line_.substr(range->pos, range->len);
        return *this;
    }

    auto accept_until(std::string_view& result, const std::regex& sep_pattern) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        std::string_view remain = str::take_view(line_, rpos_);
        std::match_results<std::string_view::const_iterator> match;
        if (!std::regex_search(remain.begin(), remain.end(), match, sep_pattern)) {
            ok_ = false;
            return *this;
        }

        result = str::take_left(remain, match.position(0));
        rpos_ = match.position(0) + match.length(0);
        return *this;
    }

    auto skip_spaces(str::size_type min_n = 0) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        str::skip_spaces(line_, rpos_, min_n);
        return *this;
    }

    auto skip_max(str::size_type max_n) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        str::skip_max(line_, rpos_, max_n);
        return *this;
    }

    auto skip_n(str::size_type n) -> acceptor& {
        if (!ok_) {
            return *this;
        }

        if (!str::skip_n(line_, rpos_, n)) {
            ok_ = false;
        }
        return *this;
    }

    auto pos() const -> str::size_type {
        return rpos_;
    }

    auto range() const -> str::range_type {
        return str::range(start_, rpos_ - start_);
    }

    operator bool() const {
        return ok_;
    }

    auto ok() const -> bool {
        return ok_;
    }

private:
    std::string_view line_;
    str::size_type start_{0};
    str::size_type rpos_{0};
    bool ok_{true};
};

auto try_parse_line(render_context& context) -> void;

auto try_parse_head(render_context& context) -> void {
    static std::regex head_pattern{R"(^(#+)\s+(.*))"};
    const auto& line = context.reader.line_text();
    std::smatch match;
    std::regex_match(line, match, head_pattern);
    int32_t level = static_cast<int32_t>(match[1].length());

    node_head* head = new node_head(static_cast<int8_t>(level));
    context.parent->append(head);
    context.parent = head;
    try_parse_line(context);
    context.parent = context.parent->parent;
}

// % xxx
auto try_parse_section(render_context& context) -> void {
    static std::regex section_pattern{R"(^(%+)\s+(.*))"};

    const auto& line = context.reader.line_text();
    std::smatch match;
    std::regex_match(line, match, section_pattern);
    int32_t level = static_cast<int32_t>(match[1].length());

    node_section* section = new node_section(static_cast<int8_t>(level));
    context.parent->append(section);
    context.parent = section;
    try_parse_line(context);
    context.parent = context.parent->parent;
}

// * xxx
auto try_parse_unordered_list(render_context& context) -> void {
    static std::regex ul_pattern{R"(^(\*+)\s+(.*))"};

    const auto& line = context.reader.line_text();
    std::smatch match;
    std::regex_match(line, match, ul_pattern);
    int32_t level = static_cast<int32_t>(match[1].length());

    node_unorderd_list* ul_new = new node_unorderd_list(level);

    node* add_to_parent = nullptr;
    for (node* parent = context.parent; true; parent = parent->parent) {
        assert(parent != nullptr);
        if (parent->kind != NODE_KIND_UL) {
            add_to_parent = parent;
            break;
        }

        node_unorderd_list* ul_node = reinterpret_cast<node_unorderd_list*>(parent);
        if (ul_node->level > level) {
            add_to_parent = ul_node;
            break;
        }
    }

    assert(add_to_parent != nullptr);

    add_to_parent->append(ul_new);
    context.parent = ul_new;
    assert(context.line_parse_stack.empty());
    context.line_parse_stack.clear();
    context.line_parse_stack.emplace_back(str::take_view(line, match.position(2), match.length(2)));
    try_parse_line(context);
    assert(context.line_parse_stack.empty());

    return;
}


auto try_parse_strong(std::string_view line, str::range_type& range) -> node* {
    std::string_view range_text = str::take_view(line, range);
    acceptor acceptor(range_text);
    std::string_view content;

    if (!acceptor.from(0).accept("**").accept_until(content, "**")) {
        return nullptr;
    }

    range = acceptor.range().shift(range.pos);
    return new node_strong(content);
}

auto try_parse_em(std::string_view line, str::range_type& range) -> node* {
    acceptor acceptor(str::take_view(line, range));
    std::string_view content;

    // em
    if (!acceptor.from(0).accept("*").accept_until(content, "*")) {
        return nullptr;
    }

    range = acceptor.range().shift(range.pos);
    return new node_em(content);
}

auto try_parse_icode(std::string_view line, str::range_type& range) -> node* {
    acceptor acceptor(str::take_view(line, range));
    std::string_view content;

    if (!acceptor.from(0).accept('`').accept_until(content, '`')) {
        return nullptr;
    }

    range = acceptor.range().shift(range.pos);
    node_icode* icode = new node_icode(content);
    return icode;
}

auto try_parse_hlink(std::string_view line, str::range_type& range) -> node* {
    acceptor acceptor(str::take_view(line, range));
    std::string_view content;

    std::string_view hlink_name;
    std::string_view hlink_url;
    acceptor.from(0)
            .accept('[')
            .accept_until(hlink_name, '\\', ']')
            .skip_spaces()
            .accept('(')
            .accept_until(hlink_url, '\\', ')');
    if (!acceptor) {
        return nullptr;
    }

    range = acceptor.range().shift(range.pos);
    return new node_hlink(hlink_name, hlink_url);
}

auto try_parse_anno(std::string_view line, str::range_type& range) -> node* {
    // @xxx{} @xxx{yyy, yyy}
    // @{yyy} @{yyy, yyy}
    // @xxx yyy, yyy:
    acceptor acceptor(str::take_view(line, range));
    static std::regex name_pattern{R"([a-z]+)"};
    std::string_view anno_type;
    std::vector<std::string_view> anno_names;

    // 形式：@{yyy} @{yyy, yyy}
    if (acceptor.from(0).accept("@{")) {
        std::string_view item;
        while (true) {

            acceptor.skip_spaces();

            if (!acceptor.accept(item, name_pattern)) {
                break;
            }

            anno_names.emplace_back(item);

            acceptor.skip_spaces();

            if (acceptor.starts_with(",")) {
                continue;
            }

            if (acceptor.starts_with("}")) {
                break;
            }

            break;
        }

        if (!acceptor.starts_with("}")) {
            return nullptr;
        }

        acceptor.skip_n(1);

        range = acceptor.range().shift(range.pos);
        return new node_anno(anno_type, anno_names);
    }

    // 形式：@xxx{} @xxx{yyy,yyy}
    if (acceptor.from(0).accept("@").accept(anno_type, name_pattern).accept('{')) {
        acceptor.skip_spaces();

        while (!acceptor.starts_with("}")) {
            std::string_view name;

            acceptor.skip_spaces();

            if (!acceptor.accept(name, name_pattern)) {
                return nullptr;
            }

            acceptor.skip_spaces();

            anno_names.emplace_back(name);

            if (!acceptor.starts_with(",")) {
                break;
            }
        }

        if (!acceptor.starts_with("}")) {
            return nullptr;
        }

        acceptor.skip_n(1);

        range = acceptor.range().shift(range.pos);
        return new node_anno(anno_type, anno_names);
    }

    return nullptr;
}


auto try_parse_image(std::string_view line, str::range_type& range) -> node* {
    // ![]()
    acceptor acceptor(str::take_view(line, range));
    std::string_view image_name;
    std::string_view image_url;

    acceptor.accept("![")
            .accept_until(image_name, '\\', ']')
            .accept('(')
            .accept_until(image_url, ')');
    if (!acceptor) {
        return nullptr;
    }

    range = acceptor.range().shift(range.pos);
    return new node_image(image_name, image_url);
}

// stmt_list
//     : stmt
//     ;
//
// stmt
//     : 'begin'  stmt_list 'end'
//     | '{' stmt_list '}'
//     ;

auto try_parse_line_range(render_context& context, str::range_type range) -> void {
    const std::string& line = context.reader.line_text();

    size_t rpos = range.begin();
    size_t curr = rpos;
    while (curr < range.end()) {
        char c = line[curr];
        str::range_type parse_range{curr, (range.end() - curr)};
        switch (c) {
            case '*': {
                // **xxx**
                if (auto node = try_parse_strong(line, parse_range); node != nullptr) {
                    if (rpos < parse_range.begin()) {
                        node_text* text = new node_text(str::take_view(line, str::range(rpos, (parse_range.begin() - rpos))));
                        context.parent->append(text);
                    }

                    context.parent->append(node);
                    rpos = curr = parse_range.end();
                    continue;
                }

                // *xxx*
                if (auto node = try_parse_em(line, parse_range); node != nullptr) {
                    continue;
                }
            }
            break;
            case '`': {
                if (auto node = try_parse_icode(line, parse_range); node != nullptr) {
                    if (rpos < parse_range.begin()) {
                        node_text* text = new node_text(str::take_view(line, str::range(rpos, (parse_range.begin() - rpos))));
                        context.parent->append(text);
                    }

                    context.parent->append(node);
                    rpos = curr = parse_range.end();
                    continue;
                }
            }
            break;
            case '[': {
                if (auto node = try_parse_hlink(line, parse_range); node != nullptr) {
                    if (rpos < parse_range.begin()) {
                        node_text* text = new node_text(str::take_view(line, str::range(rpos, (parse_range.begin() - rpos))));
                        context.parent->append(text);
                    }

                    context.parent->append(node);
                    rpos = curr = parse_range.end();
                    continue;
                }
            }
            break;
            case '@': {
                if (auto node = try_parse_anno(line, parse_range); node != nullptr) {
                    if (rpos < parse_range.begin()) {
                        node_text* text = new node_text(str::take_view(line, str::range(rpos, (parse_range.begin() - rpos))));
                        context.parent->append(text);
                    }

                    context.parent->append(node);
                    rpos = curr = parse_range.end();
                    continue;
                }
            }
            break;
            case '!': {
                if (auto node = try_parse_image(line, parse_range); node != nullptr) {
                    if (rpos < parse_range.begin()) {
                        node_text* text = new node_text(str::take_view(line, str::range(rpos, (parse_range.begin() - rpos))));
                        context.parent->append(text);
                    }

                    context.parent->append(node);
                    rpos = curr = parse_range.end();
                    continue;
                }
            }
            break;
            default: {
                // 扫描字符串行，知道遇到指定的分隔符
                auto text_range = str::accept_until(line, curr, str::charset("*`[@!"));
                curr = (text_range ? text_range->end() : parse_range.end());
            }
            break;
        }
    }

    // 这一行最后一部分
    if (rpos < curr) {
        node_text* text = new node_text(str::take_view(line, str::range(rpos, (curr - rpos))));
        context.parent->append(text);
        context.rpos = curr;
    }
}

auto try_parse_line(render_context& context) -> void {
    try_parse_line_range(context, str::range(0, context.reader.line_text().size()));
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
    } while (context.reader.next_line());

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
    // static std::regex param_pattern1{R"(^\s*@param\s+([a-zA-Z_][0-9a-zA-Z_]*)(,\s+([a-zA-Z_][0-9a-zA-Z_]*))?:(.*))"};
    // static std::regex param_pattern2{R"(^\s*@param\{([a-zA-Z_][0-9a-zA-Z_]*)(,\s+[a-zA-Z_][0-9a-zA-Z_]*)?\}(.*))"};
    //
    // std::smatch match;
    // if (std::regex_match(context.reader.line_text(), match, param_pattern1)) {
    //
    // } else if (std::regex_match(context.reader.line_text(), match, param_pattern2)) {
    // }
    node_param* param = new node_param;
    context.parent->append(param);
    context.parent = param;

    const std::string& line = context.reader.line_text();
    str::range_type range{0, line.size()};

    // 形式：@xxx yyy, yyy:
    acceptor acceptor(str::take_view(line, range));
    static std::regex name_pattern{R"([a-z]+)"};
    std::string_view anno_type;
    std::vector<std::string_view> anno_names;

    acceptor.from(0).accept("@param");
    std::string_view item;
    while (true) {
        if (!acceptor.skip_spaces(1)) {
            assert(false);  //  格式错误: param 后面无空白
            return;
        }

        if (!acceptor.accept(item, name_pattern)) {
            assert(false);  //  格式错误：param 的参数名称不满足规范
            return;
        }

        acceptor.skip_spaces();

        if (acceptor.starts_with(",")) {
            acceptor.skip_n(1);
            continue;
        }

        break;
    }

    if (!acceptor.starts_with(":")) {
        assert(false); // 格式错误
        return;
    }

    acceptor.skip_n(1);

    // 前面是一个注解
    auto anno = new node_anno(anno_type, anno_names);
    context.parent->append(anno);

    // 后面是一个行内的元素
    auto param_list_range = acceptor.range().shift(range.pos);
    try_parse_line_range(context, str::range(param_list_range.end(), line.size() - param_list_range.end()));
}

auto try_parse_return(render_context& context) -> void {
    // static std::regex return_pattern{R"(^\s*@return:\s+(.*))"};
    // node_return* nret = new node_return;
    // context.parent->append(nret);
    node_return* ret = new node_return;
    context.parent->append(ret);
    context.parent = ret;
    try_parse_line(context);
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

    if (!opts.output_file.empty()) {
        std::string output_directory = str::dirname(opts.output_file);
        if (!std::filesystem::exists(opts.output_file)) {
            std::error_code error;
            if (!std::filesystem::create_directories(output_directory, error)) {
                std::cerr << "Can not create output directory `" << output_directory << "'" << std::endl;
                return 1;
            }
        }
    }

    if (!std::filesystem::exists(opts.root_directory)) {
        std::cerr << "The project root directory is not exist `" << opts.root_directory << "'" << std::endl;
        return 1;
    }

    FILE* input_repl = ((opts.input_file.empty()) ? stdin : nullptr);
    str::with_file(opts.input_file, "r", input_repl, [&opts, &error](FILE* ifile) -> void {
        assert(ifile != nullptr);
        FILE* output_repl = ((opts.output_file.empty()) ? stdout : nullptr);
        str::with_file(opts.output_file, "w+", output_repl, [ifile, &opts, &error](FILE* ofile) -> void {
            render_context context{ifile, ofile, opts.root_directory};
            error = render_one_file(context);
            print_md(context.root, [](std::string_view text) {
                std::cout << text;
                std::cout.flush();
            });
        });
    });

    if (!error.empty()) {
        std::cerr << "Generate document failed: " << error << std::endl;
        return 1;
    }

    return 0;
}