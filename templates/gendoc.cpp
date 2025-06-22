#include "str.hpp"

#include <cassert>
#include <filesystem>
#include <iostream>
#include <list>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

//! 节点类型表
///
/// 用法：DEF_NODEKIND(Priority_, Id_, Name_, Type_, Desc_)
#define NODEKIND_TABLE()                                       \
    /* 容器 */                                                 \
    DEF_NODEKIND(0, 10, PROJECT, node_project, "项目根")       \
    DEF_NODEKIND(0, 11, PROJECTEND, node_projectend, "项目根") \
    DEF_NODEKIND(1, 12, ARTICLE, node_article, "文章")         \
    DEF_NODEKIND(1, 13, ARTICLEEND, node_articlend, "文章")    \
    DEF_NODEKIND(2, 14, CHAPTER, node_chapter, "章")           \
    DEF_NODEKIND(2, 15, CHAPTEREND, node_chapterend, "章")     \
    DEF_NODEKIND(3, 16, SECTION, node_section, "节")           \
    DEF_NODEKIND(3, 17, SECTIONEND, node_sectionend, "节")     \
    /* 段落：会挂子节点 */                                     \
    DEF_NODEKIND(4, 20, PARAM, node_param, "参数定义")         \
    DEF_NODEKIND(4, 21, RETURN, node_return, "返回值")         \
    DEF_NODEKIND(4, 22, LIST, node_list, "列表块")             \
    /* 段落：不挂子节点 */                                     \
    DEF_NODEKIND(5, 30, TABLE, node_table, "表块")             \
    DEF_NODEKIND(5, 31, PARAGRAPH, node_paragraph, "通用段落") \
    DEF_NODEKIND(5, 32, BCODE, node_bcode, "代码块")           \
    DEF_NODEKIND(5, 33, BFORMULA, node_bformula, "行间公式")   \
    DEF_NODEKIND(6, 34, COMMENT, node_comment, "注释行")       \
    /* 行内元素 */                                             \
    DEF_NODEKIND(7, 50, IFORMULA, node_iformula, "行内公式")   \
    DEF_NODEKIND(7, 51, HLINK, node_hlink, "超链接")           \
    DEF_NODEKIND(7, 52, IMAGE, node_image, "图片")             \
    DEF_NODEKIND(7, 53, ANCHOR, node_anchor, "锚点定义")       \
    DEF_NODEKIND(7, 54, EMBDED, node_embded, "嵌入文字")       \
    DEF_NODEKIND(7, 55, STRONG, node_strong, "强调")           \
    DEF_NODEKIND(7, 56, EM, node_em, "重要")                   \
    DEF_NODEKIND(7, 57, TEXT, node_text, "文本")               \
    DEF_NODEKIND(7, 58, ICODE, node_icode, "行内代码")         \
    DEF_NODEKIND(7, 59, ANNO, node_anno, "注解")               \
    /* 特殊节点：行本身 */                                     \
    DEF_NODEKIND(20, 90, LINE, node_line, "行")                \
    DEF_NODEKIND(20, 91, THEAD, node_thead, "表头行")          \
    DEF_NODEKIND(20, 92, TROW, node_trow, "表的行")            \
    DEF_NODEKIND(20, 93, TCOL, node_tcol, "表的列")            \
    /* (end) */

enum node_kind : int8_t {
    // 错误（用于初始化）
    NODE_KIND_ERROR = 0,

#define DEF_NODEKIND(Priority_, Id_, Name_, Type_, Desc_) NODE_KIND_##Name_ = (Id_),
    NODEKIND_TABLE()
#undef DEF_NODEKIND
};

enum node_priority : int8_t {
#define DEF_NODEKIND(Priority_, Id_, Name_, Type_, Desc_) NODE_PRIORITY_##Name_ = (Priority_),
    NODEKIND_TABLE()
#undef DEF_NODEKIND
};

inline auto node_priority_of(node_kind kind) -> node_priority {
    switch (kind) {
#define DEF_NODEKIND(Priority_, Id_, Name_, Type_, Desc_) \
    case node_kind::NODE_KIND_##Name_:                    \
        return node_priority::NODE_PRIORITY_##Name_;      \
        /* (end) */
        NODEKIND_TABLE()
#undef DEF_NODEKIND
        default:
            return node_priority{-1};
    }
}

enum node_limits {
    CHAPTER_LEVEL_MIN = 1,
    CHAPTER_LEVEL_MAX = 6,

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

template <typename H>
static inline auto list_is_empty(H* head) -> bool {
    return reinterpret_cast<H*>(head->next) == head;
}

template <typename H>
static inline auto list_first(H* head) -> decltype(H::next) {
    return head->next;
}

template <typename H>
static inline auto list_last(H* head) -> decltype(H::prev) {
    return head->prev;
}

template <typename T>
static inline auto list_next(T* itr) -> decltype(T::next) {
    return itr->next;
}

template <typename T>
static inline auto list_prev(T* itr) -> decltype(T::prev) {
    return itr->prev;
}

template <typename H>
static inline auto list_end(H* head) -> const decltype(H::prev) {
    return reinterpret_cast<const decltype(H::prev)>(head);
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
    node_kind kind{NODE_KIND_ERROR};

    // 节点标记位
    uint32_t flags{0}; // node_flags

    // 符号优先级
    int8_t priority{0};

    // 层级(如果有的话)
    int8_t level{0};

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

struct node_project : public node {
    explicit node_project() {
        kind = NODE_KIND_PROJECT;
        priority = NODE_PRIORITY_PROJECT;
    }
};

struct node_projectend : public node {
    explicit node_projectend() {
        kind = NODE_KIND_PROJECTEND;
        priority = NODE_PRIORITY_PROJECTEND;
    }
};

struct node_image;
struct node_hlink;
struct node_anno;
struct node_anchor;

struct node_articleend : public node {
    explicit node_articleend() {
        kind = NODE_KIND_ARTICLEEND;
        priority = NODE_PRIORITY_ARTICLEEND;
    }
};

struct node_chapter : public node {
    explicit node_chapter(int8_t l) {
        assert((l >= CHAPTER_LEVEL_MIN) && (l <= CHAPTER_LEVEL_MAX));
        kind = NODE_KIND_CHAPTER;
        priority = NODE_PRIORITY_CHAPTER;
        level = l;
    }
};

struct node_chapterend : public node {
    explicit node_chapterend(int8_t l) {
        assert((l >= CHAPTER_LEVEL_MIN) && (l <= CHAPTER_LEVEL_MAX));
        kind = NODE_KIND_CHAPTEREND;
        priority = NODE_PRIORITY_CHAPTEREND;
        level = l;
    }
};

struct node_section : public node {
    explicit node_section(int8_t l) {
        assert((l >= SECTION_LEVEL_MIN) && (l <= SECTION_LEVEL_MAX));
        kind = NODE_KIND_SECTION;
        priority = NODE_PRIORITY_SECTION;
        level = l;
    }
};

struct node_sectionend : public node {
    explicit node_sectionend(int8_t l) {
        kind = NODE_KIND_SECTIONEND;
        priority = NODE_PRIORITY_SECTIONEND;
        level = l;
    }
};

struct node_paragraph : public node {
    explicit node_paragraph() {
        kind = NODE_KIND_PARAGRAPH;
        priority = NODE_PRIORITY_PARAGRAPH;
    }
};

struct node_line : public node {
    int32_t line_no{};

    explicit node_line() {
        kind = NODE_KIND_LINE;
        priority = NODE_PRIORITY_LINE;
    }
};

struct node_comment : public node {
    std::vector<std::string> lines;

    explicit node_comment() {
        kind = NODE_KIND_COMMENT;
        priority = NODE_PRIORITY_COMMENT;
    }
};

struct node_bcode : public node {
    std::string language;
    std::map<std::string, std::string> properties;
    std::vector<std::string> lines;

    explicit node_bcode(std::string_view lang, std::map<std::string, std::string> prop) {
        kind = NODE_KIND_BCODE;
        priority = NODE_PRIORITY_BCODE;
        language = lang;
        properties = std::move(prop);
    }
};

struct node_icode : public node {
    std::string text;

    explicit node_icode(std::string_view content) {
        kind = NODE_KIND_ICODE;
        priority = NODE_PRIORITY_ICODE;
        flags |= node_flags::FLAG_INLINE;
        text = content;
    }
};

struct node_iformula : public node {
    std::string text;

    explicit node_iformula() {
        kind = NODE_KIND_IFORMULA;
        priority = NODE_PRIORITY_IFORMULA;
    }
};

struct node_bformula : public node {
    std::vector<std::string> lines;

    explicit node_bformula() {
        kind = NODE_KIND_BFORMULA;
        priority = NODE_PRIORITY_BFORMULA;
    }
};

struct node_embded : public node {
    std::string file_name;
    std::string block_name;

    explicit node_embded() {
        kind = NODE_KIND_EMBDED;
        priority = NODE_PRIORITY_EMBDED;
    }
};

struct node_list : public node {
    explicit node_list(int8_t l) {
        kind = NODE_KIND_LIST;
        priority = NODE_PRIORITY_LIST;
        level = l;
    }
};

struct node_anchor : public node {
    std::vector<std::string> names;

    explicit node_anchor() {
        kind = NODE_KIND_ANCHOR;
        priority = NODE_PRIORITY_ANCHOR;
    }
};

struct node_param : public node {
    std::vector<std::string> names;

    explicit node_param() {
        kind = NODE_KIND_PARAM;
        priority = NODE_PRIORITY_PARAM;
    }

    explicit node_param(std::vector<std::string_view> n)
        : node_param() {
        for (auto item : n) {
            names.emplace_back(item);
        }
    }
};

struct node_return : public node {
    explicit node_return() {
        kind = NODE_KIND_RETURN;
        priority = NODE_PRIORITY_RETURN;
    }
};

struct node_text : public node {
    std::string text;

    explicit node_text(std::string_view content) {
        kind = NODE_KIND_TEXT;
        priority = NODE_PRIORITY_TEXT;
        text = content;
    }
};

struct node_strong : public node {
    std::string text;

    explicit node_strong(std::string_view content) {
        kind = NODE_KIND_STRONG;
        priority = NODE_PRIORITY_STRONG;
        text = content;
    }
};

struct node_em : public node {
    std::string text;

    explicit node_em(std::string_view content) {
        kind = NODE_KIND_EM;
        priority = NODE_PRIORITY_EM;
        text = content;
    }
};

struct node_hlink : public node {
    std::string name;
    std::string url;

    explicit node_hlink(std::string_view n, std::string_view u) {
        kind = NODE_KIND_HLINK;
        priority = NODE_PRIORITY_HLINK;
        name = n;
        url = u;
    }
};

struct node_image : public node {
    std::string name;
    std::string url;

    explicit node_image(std::string_view n, std::string_view u) {
        kind = NODE_KIND_IMAGE;
        priority = NODE_PRIORITY_IMAGE;
        name = n;
        url = u;
    }
};

struct node_anno : public node {
    std::string type;
    std::vector<std::string> names;

    explicit node_anno(std::string_view n, const std::vector<std::string_view>& u) {
        kind = NODE_KIND_ANNO;
        priority = NODE_PRIORITY_ANNO;
        type = n;
        for (auto item : u) {
            names.emplace_back(item);
        }
    }
};

struct node_article : public node {
    std::string root_directory;
    std::string filepath;
    std::list<node_image*> images;
    std::list<node_hlink*> hlinks;
    std::list<node_anno*> refs;
    std::map<std::string, node_anchor*> anchors;

    explicit node_article() {
        kind = NODE_KIND_ARTICLE;
        priority = NODE_PRIORITY_ARTICLE;
    }

    // register anchors
    inline auto register_anchor(node_anchor* anchor) -> void {
        assert(anchor != nullptr);
        for (const auto& name : anchor->names) {
            auto itr = std::as_const(anchors).find(name);
            if (itr != anchors.cend()) {
                // TODO need to print log：anchor redefined
                continue;
            }
            anchors.emplace(name, anchor);
        }
    }

    // register unresolved refer
    inline auto register_ref(node_anno* ref) -> void {
        assert(ref != nullptr);
        if ((ref->type != "ref") && (ref->type != "")) {
            // 出现这种情况，通常是代码有 bug 了
            assert(false);
            return;
        }

        refs.emplace_back(ref);
    }

    inline auto register_hlink(node_hlink* hlink) -> void {
        assert(hlink != nullptr);
        hlinks.emplace_back(hlink);
    }

    inline auto register_image(node_image* image) -> void {
        assert(image != nullptr);
        images.emplace_back(image);
    }
};

auto print_tree(node* nd, size_t ident, const std::function<void(std::string_view)> print) -> void {
    assert((nd != nullptr) && (print != nullptr));
    switch (nd->kind) {
        case NODE_KIND_PROJECT: {
            print(str::make_spaces(ident * 4));
            node_project* project = reinterpret_cast<node_project*>(nd);
            for (auto child = project->children.next; child != reinterpret_cast<node*>(&project->children); child = child->next) {
                print_tree(child, ident, print);
            }
        } break;
        case NODE_KIND_ARTICLE: {
            print(str::make_spaces(ident * 4));
            node_article* article = reinterpret_cast<node_article*>(nd);
            for (auto child = article->children.next; child != reinterpret_cast<node*>(&article->children); child = child->next) {
                print_tree(child, ident, print);
            }
        } break;
        case NODE_KIND_CHAPTER: {
            print(str::make_spaces(ident * 4));
            node_chapter* chapter = reinterpret_cast<node_chapter*>(nd);
            print(str::repeat("#", chapter->level));
            print(" ");
            auto line = chapter->children.next;
            print_tree(line, 0, print); // 0: 不需要加空白前缀
            print("\n");
            for (auto child = line->next; child != reinterpret_cast<node*>(&chapter->children); child = child->next) {
                print_tree(child, ident + 1, print);
            }
        } break;
        case NODE_KIND_SECTION: {
            print(str::make_spaces(ident * 4));
            node_section* section = reinterpret_cast<node_section*>(nd);
            auto tag = str::repeat("%", section->level);
            print(tag);
            print(" ");
            auto line = list_first(&section->children);
            if (line != list_end(&section->children)) {
                assert(line->kind == NODE_KIND_LINE);
                print_tree(line, 0, print); // 0: 不需要加空白前缀
                print("\n");
                for (auto child = list_next(line); child != list_end(&section->children); child = list_next(child)) {
                    print_tree(child, ident + 1, print);
                }
                print("\n");
            }
        } break;
        case NODE_KIND_PARAGRAPH: {
            node_paragraph* paragraph = reinterpret_cast<node_paragraph*>(nd);
            for (auto child = paragraph->children.next; child != reinterpret_cast<node*>(&paragraph->children); child = child->next) {
                print_tree(child, ident, print);
            }
            print("\n");
        } break;
        case NODE_KIND_LINE: {
            print(str::make_spaces(ident * 4));
            node_line* line = reinterpret_cast<node_line*>(nd);
            for (auto child = line->children.next; child != reinterpret_cast<node*>(&line->children); child = child->next) {
                print_tree(child, ident + 1, print);
            }
            print("\n");
        } break;
        case NODE_KIND_COMMENT: {
            print(str::make_spaces(ident * 4));
            node_comment* comment = reinterpret_cast<node_comment*>(nd);
            print("<!-- \n");
            for (auto& line : comment->lines) {
                print(line);
                print("\n");
            }
            print("-->\n");
        } break;
        case NODE_KIND_THEAD: {
        } break;
        case NODE_KIND_TROW: {

        } break;
        case NODE_KIND_TCOL: {

        } break;
        case NODE_KIND_PARAM: {
            print(str::make_spaces(ident * 4));
            node_param* param = reinterpret_cast<node_param*>(nd);
            print("@param ");
            for (size_t index = 0; index < param->names.size(); index++) {
                if (index != 0) {
                    print(", ");
                }
                print(param->names[index]);
            }
            print(": ");
            auto child = list_first(&param->children);
            if (child != list_end(&param->children)) {
                assert(child->kind == NODE_KIND_LINE); // 第一行必定是 line
                print_tree(child, 0, print);
                print("\n");
                // 其余的都是子节点
                for (child = list_next(child); child != list_end(&param->children); child = list_next(child)) {
                    print_tree(child, ident + 1, print);
                }
            }
            print("\n");
        } break;
        case NODE_KIND_RETURN: {
            print(str::make_spaces(ident * 4));
            print("@return: ");
            node_return* ret = reinterpret_cast<node_return*>(nd);
            auto child = list_first(&ret->children);
            if (child != list_end(&ret->children)) {
                assert(child->kind == NODE_KIND_LINE); // 第一行必定是 line
                print_tree(child, 0, print);
                print("\n");
                // 其余的都是子节点
                for (child = list_next(child); child != list_end(&ret->children); child = list_next(child)) {
                    print_tree(child, ident + 1, print);
                }
            }
            print("\n");
        } break;
        case NODE_KIND_BCODE: {
            print(str::make_spaces(ident * 4));
            node_bcode* bcode = reinterpret_cast<node_bcode*>(nd);
            print("```");
            print(bcode->language);
            if (!bcode->properties.empty()) {
                print(": ");
                for (auto itr = bcode->properties.begin(); itr != bcode->properties.end(); itr++) {
                    if (itr != bcode->properties.begin()) {
                        print(", ");
                    }
                    print(itr->first);
                    print("=");
                    print(itr->second);
                }
            }
            print("\n");
            for (auto& line : bcode->lines) {
                print(str::make_spaces(ident * 4));
                print(line);
                print("\n");
            }
            print(str::make_spaces(ident * 4));
            print("```\n");
        } break;
        case NODE_KIND_IFORMULA: {
            node_iformula* iformula = reinterpret_cast<node_iformula*>(nd);
            print("$");
            print(iformula->text);
            print("$");
        } break;
        case NODE_KIND_BFORMULA: {
            node_bformula* bformula = reinterpret_cast<node_bformula*>(nd);
            print("$$\n");
            for (auto& line : bformula->lines) {
                print(line);
                print("\n");
            }
            print("$$\n");
        } break;
        case NODE_KIND_LIST: {
            print(str::make_spaces(ident * 4));
            node_list* list = reinterpret_cast<node_list*>(nd);
            print(str::repeat("*", list->level));
            print(" ");
            auto first = list_first(&list->children);
            if (first != list_end(&list->children)) {
                print_tree(first, 0, print);
                print("\n");
                for (auto child = list_next(first); child != list_end(&list->children); child = list_next(child)) {
                    print_tree(child, ident + 1, print);
                }
            }
            // for (auto child = list_first(&list->children); child != list_end(&list->children); child = list_next(child)) {
            //     print_md(child, level, print);
            // }
        } break;
        // case NODE_KIND_LISTITEM: {
        //     node_listitem* listitem = reinterpret_cast<node_listitem*>(nd);
        //     print(str::repeat("*", listitem->level));
        //     print(" ");
        //     auto first = list_first(&listitem->children);
        //     if (first != list_end(&listitem->children)) {
        //         print_md(first, 0, print);
        //         print("\n");
        //         for (auto child = list_next(first); child != list_end(&listitem->children); child = list_next(child)) {
        //             print_md(child, level + 1, print);
        //         }
        //     }
        // }
        // break;
        case NODE_KIND_HLINK: {
            node_hlink* hlink = reinterpret_cast<node_hlink*>(nd);
            print("[");
            print(hlink->name);
            print("](");
            print(hlink->url);
            print(")");
        } break;
        case NODE_KIND_IMAGE: {
            node_image* image = reinterpret_cast<node_image*>(nd);
            print("![");
            print(image->name);
            print("](");
            print(image->url);
            print(")");
        } break;
        case NODE_KIND_ANCHOR: {

        } break;
        case NODE_KIND_EMBDED: {

        } break;
        case NODE_KIND_STRONG: {
            node_strong* strong = reinterpret_cast<node_strong*>(nd);
            print("**");
            print(strong->text);
            print("**");
        } break;
        case NODE_KIND_EM: {
            node_em* em = reinterpret_cast<node_em*>(nd);
            print("*");
            print(em->text);
            print("*");
        } break;
        case NODE_KIND_TEXT: {
            node_text* text = reinterpret_cast<node_text*>(nd);
            print(text->text);
        } break;
        case NODE_KIND_ICODE: {
            node_icode* text = reinterpret_cast<node_icode*>(nd);
            print("`");
            print(text->text);
            print("`");
        } break;
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
        } break;
        default: {
            assert(false);
        } break;
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
    NORMAL,
    ENTER_PARAGRAPH,
    // ENTER_LIST,
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

    inline auto rollback() -> void {
        cached_lines_.emplace_back(std::move(line_));
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

using svmatch = std::match_results<std::string_view::const_iterator>;

struct render_context {
    explicit render_context(FILE* ifile, FILE* ofile, const std::string& rd)
        : root_directory_{rd}
        , reader_{ifile}
        , writer_{ofile} {
        project_ = new node_project;
        parent_ = project_;
    }

    ~render_context() {
        delete project_;
    }

    inline auto rpos() const -> size_t {
        return rpos_;
    }

    inline auto rpos(size_t rp) -> void {
        rpos_ = rp;
    }

    inline auto rollback_line() -> void {
        reader_.rollback();
    }

    inline auto rollback_line(size_t ln, std::string_view lt) -> void {
        reader_.rollback(ln, lt);
    }

    inline auto line_no() const -> size_t {
        return reader_.line_no();
    }

    inline auto line_text() const -> const std::string& {
        return reader_.line_text();
    }

    inline auto push(node* child) -> void {
        parent_->append(child);
        parent_ = child;
    }

    inline auto pop() -> node* {
        parent_ = parent_->parent;
        return parent_;
    }

    inline auto append_child(node* child) -> void {
        assert(child != nullptr);
        parent_->append(child);
    }

    inline auto parent(node* p) -> void {
        assert(p != nullptr);
        parent_ = p;
    }

    inline auto parent() -> node* {
        return parent_;
    }

    inline auto parent() const -> const node* {
        return parent_;
    }

    auto backtrack_parent(const std::function<bool(node* node)>& proc) -> node* {
        node* curr = parent_;
        while (!proc(curr)) {
            if (curr->parent == nullptr) {
                return nullptr;
            }

            curr = curr->parent;
        }

        return curr;
    }

    inline auto next_line() -> bool {
        return reader_.next_line();
    }

    inline auto root() -> node* {
        return project_;
    }

    inline auto root() const -> const node* {
        return project_;
    }

    inline auto state() const -> parse_state {
        return state_;
    }

    inline auto state(parse_state st) -> void {
        state_ = st;
    }

private:
    const std::string& root_directory_;
    line_reader reader_;
    FILE* writer_{nullptr};

    node_project* project_{nullptr};
    node_article* article_{nullptr};
    node* parent_{nullptr};

    size_t rpos_{0};
    parse_state state_{parse_state::NORMAL};
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

auto try_parse_line_range(render_context& context, str::range_type range) -> void;

auto try_parse_chapter(render_context& context, int8_t level, str::range_type range) -> void {
    auto add_to_parent = context.parent();
    while (true) {
        if (add_to_parent->priority < node_priority_of(NODE_KIND_CHAPTER)) {
            break;
        }

        if (add_to_parent->priority == node_priority_of(NODE_KIND_CHAPTER)) {
            if (add_to_parent->level < level) {
                break;
            }
        }

        add_to_parent = add_to_parent->parent;
    }

    context.parent(add_to_parent);
    context.push(new node_chapter(static_cast<int8_t>(level)));
    context.push(new node_line);

    try_parse_line_range(context, range);

    context.pop();
}

// % xxx
auto try_parse_section(render_context& context, int32_t level, str::range_type range) -> void {
    auto add_to_parent = context.parent();
    while (true) {
        if (add_to_parent->priority < node_priority_of(NODE_KIND_SECTION)) {
            break;
        }

        if (add_to_parent->priority == node_priority_of(NODE_KIND_SECTION)) {
            if (add_to_parent->level < level) {
                break;
            }
        }

        add_to_parent = add_to_parent->parent;
    }

    context.parent(add_to_parent);
    context.push(new node_section(static_cast<int8_t>(level)));
    context.push(new node_line);

    try_parse_line_range(context, range);

    context.pop();
}

// * xxx
auto try_parse_list(render_context& context, int8_t level, str::range_type range) -> void {
    auto add_to_parent = context.parent();
    while (true) {
        if (add_to_parent->priority < node_priority_of(NODE_KIND_LIST)) {
            break;
        }

        if (add_to_parent->priority == node_priority_of(NODE_KIND_LIST)) {
            if (add_to_parent->level < level) {
                break;
            }
        }

        add_to_parent = add_to_parent->parent;
    }

    context.parent(add_to_parent);
    // context.push(new node_listitem(level));
    context.push(new node_list(level));
    context.push(new node_line());
    try_parse_line_range(context, range);
    context.pop();
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

auto try_parse_line_range(render_context& context, str::range_type range) -> void {
    const std::string& line = context.line_text();

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
                        context.append_child(text);
                    }

                    context.append_child(node);
                    rpos = curr = parse_range.end();
                    continue;
                }

                // *xxx*
                if (auto node = try_parse_em(line, parse_range); node != nullptr) {
                    continue;
                }
            } break;
            case '`': {
                if (auto node = try_parse_icode(line, parse_range); node != nullptr) {
                    if (rpos < parse_range.begin()) {
                        node_text* text = new node_text(str::take_view(line, str::range(rpos, (parse_range.begin() - rpos))));
                        context.append_child(text);
                    }

                    context.append_child(node);
                    rpos = curr = parse_range.end();
                    continue;
                }
            } break;
            case '[': {
                if (auto node = try_parse_hlink(line, parse_range); node != nullptr) {
                    if (rpos < parse_range.begin()) {
                        node_text* text = new node_text(str::take_view(line, str::range(rpos, (parse_range.begin() - rpos))));
                        context.append_child(text);
                    }

                    context.append_child(node);
                    rpos = curr = parse_range.end();
                    continue;
                }
            } break;
            case '@': {
                if (auto node = try_parse_anno(line, parse_range); node != nullptr) {
                    if (rpos < parse_range.begin()) {
                        node_text* text = new node_text(str::take_view(line, str::range(rpos, (parse_range.begin() - rpos))));
                        context.append_child(text);
                    }

                    context.append_child(node);
                    rpos = curr = parse_range.end();
                    continue;
                }
            } break;
            case '!': {
                if (auto node = try_parse_image(line, parse_range); node != nullptr) {
                    if (rpos < parse_range.begin()) {
                        node_text* text = new node_text(str::take_view(line, str::range(rpos, (parse_range.begin() - rpos))));
                        context.append_child(text);
                    }

                    context.append_child(node);
                    rpos = curr = parse_range.end();
                    continue;
                }
            } break;
            default: {
                // 扫描字符串行，知道遇到指定的分隔符
                auto text_range = str::accept_until(line, curr, str::charset("*`[@!"));
                curr = (text_range ? text_range->end() : parse_range.end());
            } break;
        }

        // 从下一个字符开始解析
        curr++;

        // 扫描字符串行，知道遇到指定的分隔符
        auto text_range = str::accept_until(line, curr, str::charset("*`[@!"));
        curr = (text_range ? text_range->end() : parse_range.end());
    }

    // 这一行最后一部分
    if (rpos < curr) {
        node_text* text = new node_text(str::take_view(line, str::range(rpos, (curr - rpos))));
        context.append_child(text);
        context.rpos(curr);
    }
}

auto try_parse_line(render_context& context) -> void {
    context.push(new node_line);

    try_parse_line_range(context, str::range(0, context.line_text().size()));

    context.pop();
}

auto try_parse_comment(render_context& context) -> void {
    node_comment* comment = new node_comment;
    context.append_child(comment);

    do {
        comment->lines.emplace_back(context.line_text());
        if (str::ends_with(context.line_text(), "-->")) {
            return;
        }
    } while (context.next_line());
}

auto try_parse_param(render_context& context) -> void {
    const std::string& line = context.line_text();
    str::range_type range{0, line.size()};

    // 形式：@xxx yyy, yyy:
    acceptor acceptor(str::take_view(line, range));
    static std::regex name_pattern{R"([a-z]+)"};
    std::vector<std::string_view> anno_names;

    acceptor.from(0).accept("@param");
    std::string_view item;
    while (true) {
        if (!acceptor.skip_spaces(1)) {
            assert(false); // 格式错误: param 后面无空白
            return;
        }

        if (!acceptor.accept(item, name_pattern)) {
            assert(false); // 格式错误：param 的参数名称不满足规范
            return;
        }

        anno_names.emplace_back(item);

        acceptor.skip_spaces();

        if (acceptor.starts_with(",")) {
            acceptor.skip_n(1);
            continue;
        }

        break;
    }

    if (!acceptor.accept_until(":")) {
        assert(false); // 格式错误
        return;
    }

    context.push(new node_param(anno_names));

    // 后面是一个行内的元素
    auto param_list_range = acceptor.range().shift(range.pos);
    try_parse_line_range(context, str::range(param_list_range.end(), line.size() - param_list_range.end()));
}

auto try_parse_return(render_context& context, str::range_type range) -> void {
    context.push(new node_return);
    context.push(new node_line);

    try_parse_line_range(context, range);

    context.pop(); // pop line
    context.pop(); // pop return
}

auto try_parse_bcode(render_context& context) -> void {
    auto& line = context.line_text();

    // 识别语言识别符
    std::string language;
    size_t pos = 3; // 三个前缀符：```
    auto range = str::accept_until(line, pos, ":");
    if (!range) {
        language = str::trim_surrounding(str::take_view(line, 3));
        pos = line.size();
    } else {
        language = str::trim_surrounding(str::take_view(line, *range));
        pos += range->size();
    }

    // 识别属性表
    std::map<std::string, std::string> properties;
    std::string_view prop_text = str::take_view(line, pos);
    if (!prop_text.empty()) {
        str::split_map(prop_text, ",", "=", //
            [&properties](std::string_view key, std::string_view val) -> int {
                properties.emplace(str::trim_surrounding(key), str::trim_surrounding(val));
                return 0;
            });
    }

    node_bcode* bcode = new node_bcode(language, properties);
    context.append_child(bcode);
    while (context.next_line()) {
        if (str::starts_with(context.line_text(), "```")) {
            break;
        }

        bcode->lines.emplace_back(context.line_text());
    }
}

auto leave_atuoblock(parse_state& state, render_context& context) -> void;

auto enter_paragraph(parse_state& state, render_context& context) -> void {
    // paragraph-begin
    if (state == parse_state::NORMAL) {
        context.push(new node_paragraph());
        state = parse_state::ENTER_PARAGRAPH;
    }
}

auto leave_atuoblock(parse_state& state, render_context& context) -> void {
    // paragraph-end
    if (state != parse_state::NORMAL) {
        context.pop();
        state = parse_state::NORMAL;
    }
}

auto try_parse_listend(render_context& context, int32_t level, str::range_type range) -> void {
}

auto try_parse_article(render_context& context) -> void {
    assert(context.parent() != nullptr);
    assert(context.parent()->kind == NODE_KIND_PROJECT);

    context.push(new node_article);

    parse_state state = parse_state::NORMAL;
    svmatch matches;
    while (context.next_line()) {
        std::string_view line = context.line_text();

        // 遇到空行或者空白行，意味着块结束
        if (str::is_space_or_empty(line)) {
            // paragraph 必须是连续的，所以遇到空行需要退出block
            leave_atuoblock(state, context);

            // 消费掉所有空行、空白行
            while (context.next_line()) {
                line = context.line_text();
                if (!str::is_space_or_empty(line)) {
                    // 回退当前行，编译借用外层循环的判断条件，实现统一接口退出
                    context.rollback_line();
                    break;
                }
            }
            continue;
        }

        // @param 开头的行
        static std::regex param_pattern{R"((@param)[^a-z0-9_])"};
        if (std::regex_match(line.begin(), line.end(), matches, param_pattern)) {
            leave_atuoblock(state, context); // 立即打断当前block
            try_parse_param(context);
            continue;
        }

        // @return 开头的行
        static std::regex return_pattern{R"(^(@return)(\s+|\s*:\s*).*)"};
        if (std::regex_match(line.begin(), line.end(), matches, return_pattern)) {
            leave_atuoblock(state, context); // 立即打断当前block
            auto prefix_len = matches.length(1) + matches.length(2);
            try_parse_return(context, str::range(prefix_len, (line.size() - prefix_len)));
            continue;
        }

        // 代码块
        if (str::starts_with(line, "```")) {
            leave_atuoblock(state, context); // 立即打断当前block
            try_parse_bcode(context);
            continue;
        }

        //! 注释块: <!-- ... -->
        if (str::starts_with_word(line, "<!--")) {
            leave_atuoblock(state, context); // 立即打断当前block
            try_parse_comment(context);
            continue;
        }

        // 标题行：# 号开头的行
        static std::regex hx_pattern{R"(^(#+)(\s+).*)"};
        if (std::regex_match(line.begin(), line.end(), matches, hx_pattern)) {
            leave_atuoblock(state, context); // 立即打断当前block
            auto level = matches.length(1);
            auto space_n = matches.length(2);
            try_parse_chapter(context, static_cast<int8_t>(level), str::range((level + space_n), line.size() - (level + space_n)));
            continue;
        }

        // 回退到特定标题级别
        static std::regex back_hx_pattern{R"(^(#+)(\s?)<<(\s.*)?$)"};
        if (std::regex_match(line.begin(), line.end(), matches, back_hx_pattern)) {
            leave_atuoblock(state, context); // 立即打断当前block
            auto level = matches.length(1);
            node* parent = context.parent();
            while (true) {
                if (parent->priority < node_priority_of(NODE_KIND_CHAPTER)) {
                    break;
                }

                if (parent->priority == node_priority_of(NODE_KIND_CHAPTER)) {
                    if (parent->level < level) {
                        // TODO report warning
                        // ## xxx
                        // ###<<
                        break;
                    }

                    // 当前level的节点结束，回到当前节点的上层
                    if (parent->level == level) {
                        context.parent(parent->parent);
                        break;
                    }
                }

                parent = parent->parent;
            }
            continue;
        }

        // 章节内分级： % 号开头的行
        static std::regex sx_pattern{R"(^(%+)(\s+).*)"};
        if (std::regex_match(line.begin(), line.end(), matches, sx_pattern)) {
            leave_atuoblock(state, context); // 立即打断当前block
            auto level = matches.length(1);
            auto space_n = matches.length(2);
            try_parse_section(context, static_cast<int32_t>(level), str::range((level + space_n), line.size() - (level + space_n)));
            continue;
        }

        // 回退到特定标题级别
        static std::regex back_sx_pattern{R"(^(%+)(\s?)<<(\s.*)?$)"};
        if (std::regex_match(line.begin(), line.end(), matches, back_sx_pattern)) {
            leave_atuoblock(state, context); // 立即打断当前block
            auto level = matches.length(1);
            node* parent = context.parent();
            while (true) {
                if (parent->priority < node_priority_of(NODE_KIND_SECTION)) {
                    break;
                }

                if (parent->priority == node_priority_of(NODE_KIND_SECTION)) {
                    if (parent->level < level) {
                        // TODO report warning
                        // %% xxx
                        // %%%<<
                        break;
                    }

                    if (parent->level == level) {
                        context.parent(parent->parent);
                        break;
                    }
                }

                parent = parent->parent;
            }
            continue;
        }

        // 遇到列表行
        static std::regex list_pattern{R"(^(\*+)(\s+).*)"};
        if (std::regex_match(line.begin(), line.end(), matches, list_pattern)) {
            leave_atuoblock(state, context);

            auto level = matches.length(1);
            auto space_n = matches.length(2);
            try_parse_list(context, static_cast<int8_t>(level), str::range((level + space_n), line.size() - (level + space_n)));
            continue;
        }

        // 回退到特定标题级别
        static std::regex back_list_pattern{R"(^(\*+)(\s?)<<(\s.*)?$)"};
        if (std::regex_match(line.begin(), line.end(), matches, back_list_pattern)) {
            leave_atuoblock(state, context); // 立即打断当前block
            auto level = matches.length(1);
            node* parent = context.parent();
            while (true) {
                if (parent->priority < node_priority_of(NODE_KIND_LIST)) {
                    break;
                }

                if (parent->priority == node_priority_of(NODE_KIND_LIST)) {
                    if (parent->level < level) {
                        // TODO report warning
                        // ** xxx
                        // ***<<
                        break;
                    }

                    if (parent->level == level) {
                        context.parent(parent->parent);
                        break;
                    }
                }

                parent = parent->parent;
            }
            continue;
        }

        // 可能需要进入block
        enter_paragraph(state, context);

        // 处理本行（父级一定是block，行必须位于block内才有意义）
        assert(state == parse_state::ENTER_PARAGRAPH);
        assert(context.parent()->kind == NODE_KIND_PARAGRAPH);
        try_parse_line(context);
    }
}

auto render_file(render_context& context) -> std::string {
    try_parse_article(context);
    return {};
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
            error = render_file(context);
            print_tree(context.root(), 0, [](std::string_view text) {
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