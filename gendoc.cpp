#include "str.hpp"

#include <cassert>
#include <filesystem>
#include <iostream>
#include <variant>
#include <vector>

//! 节点类型表
///
/// 用法：DEF_NODEKIND(Priority_, Name_, Desc_)
#define NODEKIND_TABLE()                 \
    DEF_NODEKIND(98, EOF, "文档结束")    \
                                         \
    DEF_NODEKIND(90, DOCUMENT, "文档根") \
    DEF_NODEKIND(80, TITLE, "文档标题")  \
                                         \
    DEF_NODEKIND(81, H1, "标题1")        \
    DEF_NODEKIND(82, H2, "标题2")        \
    DEF_NODEKIND(83, H3, "标题3")        \
    DEF_NODEKIND(84, H4, "标题4")        \
    DEF_NODEKIND(85, H5, "标题5")        \
    DEF_NODEKIND(86, H6, "标题6")        \
                                         \
    DEF_NODEKIND(71, S1, "段1")          \
    DEF_NODEKIND(72, S2, "段2")          \
    DEF_NODEKIND(73, S3, "段3")          \
    DEF_NODEKIND(74, S4, "段4")          \
                                         \
    DEF_NODEKIND(60, COMMENT, "注释")    \
    DEF_NODEKIND(60, PARAGRAPH, "段落")  \
    DEF_NODEKIND(60, THEAD, "表头")      \
    DEF_NODEKIND(60, TROW, "表的行")     \
    DEF_NODEKIND(60, TCOL, "表的列")     \
    DEF_NODEKIND(60, PARAM, "参数定义")  \
    DEF_NODEKIND(60, CODE, "代码块")     \
    DEF_NODEKIND(60, RETURN, "返回值")   \
    DEF_NODEKIND(60, BRIEF, "概要")      \
    DEF_NODEKIND(60, DETIAL, "详情")     \
    DEF_NODEKIND(60, FORMULA, "公式")    \
    DEF_NODEKIND(60, STRONG, "强调")     \
    DEF_NODEKIND(60, EM, "重要")         \
    DEF_NODEKIND(60, OL, "有序列表")     \
    DEF_NODEKIND(60, UL, "无序列表")     \
    DEF_NODEKIND(60, LI, "列表子项")     \
    DEF_NODEKIND(50, TEXT, "文本")       \
    DEF_NODEKIND(50, ICODE, "行内代码")  \
    DEF_NODEKIND(50, ECODE, "嵌入代码")  \
    DEF_NODEKIND(50, HREF, "超链接")     \
    DEF_NODEKIND(50, IMAGE, "图片")      \
    DEF_NODEKIND(50, ANCHOR, "锚点定义") \
    /* (end) */

enum node_kind : uint8_t {
#define DEF_NODEKIND(Priority_, Name_, Desc_) NODE_KIND_##Name_,
    NODEKIND_TABLE()
#undef DEF_NODEKIND
};

enum node_priority : uint8_t {
#define DEF_NODEKIND(Priority_, Name_, Desc_) NODE_PRIORITY_##Name_ = (Priority_),
    NODEKIND_TABLE()
#undef DEF_NODEKIND
};

//! 标记位
enum node_flags : uint32_t {
    FLAG_INLINE = 0x0001,     ///< 是否行内节点
    FLAG_BLOCK = 0x0002,      ///< 是否块级节点
    FLAG_STANDALONE = 0x0004, ///< 独占一行
};

struct node {
    // 节点类型
    node_kind kind{NODE_KIND_TEXT};

    // 节点标记位
    node_flags flags{0};

    // 符号优先级
    int8_t priority{0};

    // 父节点
    node* parent{nullptr};

    // 所有可能的子节点
    std::vector<node*> children;
};

struct node_document : public node {
    node_document() {
        kind = NODE_KIND_DOCUMENT;
        priority = NODE_PRIORITY_DOCUMENT;
    }
};

struct node_text : public node {
    std::string text;
};

struct node_inline_code : public node {
    std::string text;
};

struct node_embded : public node {
    std::string file_name;
    std::string block_name;
};

struct node_href : public node {
    std::string name;
    std::string url;
};

struct node_image : public node {
    std::string name;
    std::string url;
};

struct node_hr : public node {
};

struct node_anchor : public node {
    std::vector<std::string> names;
};

struct node_comment : public node {
    std::string text;
};

struct node_head : public node {
    int8_t level;
};

struct node_section : public node {
    int8_t level;
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

        auto result = str::read_next_line(file_);
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

auto try_parse_block_head(render_context& context) -> void {
}

auto try_parse_block_unordered_list(render_context& context) -> void {
}

auto try_parse_block_text(render_context& context) -> void {
}

auto try_parse_inline(render_context& context, node* parent) -> void {
}

auto try_parse_block(render_context& context) -> void {
    parse_state state = parse_state::WAIT_BLOCK;
    while (context.reader.next_line()) {
        std::string_view line = context.reader.line_text();

        if (line.empty()) {
            if (context.parent->flags & node_flags::FLAG_STANDALONE) {
                context.parent = context.parent->parent;
                continue;
            }
        }

        // 标题行：井号开头的行
        static std::regex hx_pattern{R"(^#+\s.*)"};
        if (std::regex_match(line.begin(), line.end(), hx_pattern)) {
            try_parse_block_head(context);
            return;
        }

        // 无需列表行
        static std::regex ul_pattern{R"(\*+\s.*)"};
        if (std::regex_match(line.begin(), line.end(), ul_pattern)) {
            try_parse_block_unordered_list(context);
            return;
        }

        try_parse_block_text(context);
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
