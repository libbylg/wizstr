/**
 * Copyright (c) 2021-2025  libbylg@126.com
 * str is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#ifndef GENDOC_H
#define GENDOC_H

#include <string>
#include <string_view>
#include <functional>
#include <cstdint>
#include <optional>
#include <cassert>
#include <map>
#include <vector>

namespace gendoc {
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
    DEF_NODEKIND(4, 22, NOTICE, node_notice, "注意")           \
    DEF_NODEKIND(4, 23, LIST, node_list, "列表块")             \
    /* 段落：不挂子节点 */                                     \
    DEF_NODEKIND(5, 30, TABLE, node_table, "表块")             \
    DEF_NODEKIND(5, 31, PARAGRAPH, node_paragraph, "通用段落") \
    DEF_NODEKIND(5, 32, BCODE, node_bcode, "代码块")           \
    DEF_NODEKIND(5, 33, BFORMULA, node_bformula, "行间公式")   \
    DEF_NODEKIND(5, 34, SEPARATOR, node_separator, "分隔线")   \
    DEF_NODEKIND(10,35, COMMENT, node_comment, "注释行")       \
    /* 行内元素 */                                             \
    DEF_NODEKIND(15, 50, IFORMULA, node_iformula, "行内公式")  \
    DEF_NODEKIND(15, 51, HLINK, node_hlink, "超链接")          \
    DEF_NODEKIND(15, 52, IMAGE, node_image, "图片")            \
    DEF_NODEKIND(15, 53, ANCHOR, node_anchor, "锚点定义")      \
    DEF_NODEKIND(15, 54, EMBED, node_embed, "嵌入文字")        \
    DEF_NODEKIND(15, 55, EMPHASIS, node_emphasis, "强调")      \
    DEF_NODEKIND(15, 57, TEXT, node_text, "文本")              \
    DEF_NODEKIND(15, 58, ICODE, node_icode, "行内代码")        \
    DEF_NODEKIND(15, 59, ANNO, node_anno, "注解")              \
    DEF_NODEKIND(15, 60, COLOR, node_color, "颜色")            \
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

// template <typename H>
// static inline auto list_end(H* head) -> const decltype(H::prev) {
//     return reinterpret_cast<const decltype(H::prev)>(head);
// }
template <typename H>
static inline auto list_end(H* head) -> decltype(H::prev) {
    return reinterpret_cast<decltype(H::prev)>(head);
}

#define list_foreach(Child_, List_) \
for (auto Child_ = list_first(List_); Child_ != list_end(List_); Child_ = list_next(Child_))

#define list_foreach_range(Child_, Start_, Stop_) \
for (auto Child_ = (Start_); Child_ != Stop_; Child_ = list_next(Child_))

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

struct node_separator : public node {
    std::string::value_type separator;

    explicit node_separator(std::string::value_type sep) {
        kind = NODE_KIND_SEPARATOR;
        priority = NODE_PRIORITY_SEPARATOR;
        separator = sep;
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

// @include Block:File
// @include {Block}:File
struct node_embed : public node {
    std::string file_name;
    std::string block_name;

    explicit node_embed(std::string_view b, std::string_view f) {
        kind = NODE_KIND_EMBED;
        priority = NODE_PRIORITY_EMBED;
        block_name = b;
        file_name = f;
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

struct node_notice : public node {
    explicit node_notice() {
        kind = NODE_KIND_NOTICE;
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

enum emphasis_type : uint8_t {
    ET_BOLD = 0x01,          // b
    ET_ITALIC = 0x02,        // i
    ET_UNDERLINE = 0x03,     // u
    ET_STRIKETHROUGH = 0x08, // s
};

struct node_emphasis : public node {
    std::string text;
    uint8_t types{0}; // emphasis_type

    explicit node_emphasis(uint8_t t, std::string_view content) {
        kind = NODE_KIND_EMPHASIS;
        priority = NODE_PRIORITY_EMPHASIS;
        text = content;
        types = t;
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
    std::string tag;
    std::vector<std::string> names;

    explicit node_anno(std::string_view n, const std::vector<std::string_view>& u) {
        kind = NODE_KIND_ANNO;
        priority = NODE_PRIORITY_ANNO;
        tag = n;
        for (auto item : u) {
            names.emplace_back(item);
        }
    }
};

struct node_color : public node {
    std::string color;

    explicit node_color(std::string_view c) {
        kind = NODE_KIND_COLOR;
        priority = NODE_PRIORITY_COLOR;
        color = c;
    }
};

struct node_article : public node {
    std::string root_directory;
    std::string filepath;

    explicit node_article() {
        kind = NODE_KIND_ARTICLE;
        priority = NODE_PRIORITY_ARTICLE;
    }
};

} // namespace gendoc
#endif //GENDOC_H
