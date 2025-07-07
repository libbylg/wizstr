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
#include "gendoc.h"

#include "str.hpp"

#include <cassert>
#include <filesystem>
#include <iostream>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

namespace gendoc {

static const std::string_view all_headers[]{
    "h1",
    "h2",
    "h3",
    "h4",
    "h5",
    "h6",
};

static const std::string_view all_levels[]{
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
};

class scope_guard {
public:
    explicit scope_guard(const std::function<void()>& proc)
        : proc_{proc} {
    }

    ~scope_guard() {
        if (proc_ && !dismissed_) {
            proc_();
        }
    }

    void dismiss() {
        dismissed_ = true;
    }

private:
    const std::function<void()>& proc_;
    bool dismissed_{false};
};

auto print_tree(node* nd, size_t ident, const std::function<void(std::string_view)>& print) -> void {
    assert((nd != nullptr) && (print != nullptr));
    switch (nd->kind) {
        case NODE_KIND_PROJECT: {
            print(str::make_spaces(ident * 4));
            node_project* nproject = static_cast<node_project*>(nd);
            for (auto child = nproject->children.next; child != static_cast<node*>(&nproject->children); child = child->next) {
                print_tree(child, ident, print);
            }
        } break;
        case NODE_KIND_ARTICLE: {
            print(str::make_spaces(ident * 4));
            node_article* narticle = static_cast<node_article*>(nd);
            for (auto child = narticle->children.next; child != static_cast<node*>(&narticle->children); child = child->next) {
                print_tree(child, ident, print);
            }
        } break;
        case NODE_KIND_CHAPTER: {
            print(str::make_spaces(ident * 4));
            node_chapter* nchapter = static_cast<node_chapter*>(nd);
            print(str::repeat("#", nchapter->level));
            print(" ");
            auto line = nchapter->children.next;
            print_tree(line, 0, print); // 0: 不需要加空白前缀
            print("\n");
            for (auto child = line->next; child != static_cast<node*>(&nchapter->children); child = child->next) {
                print_tree(child, ident + 1, print);
            }
        } break;
        case NODE_KIND_SECTION: {
            print(str::make_spaces(ident * 4));
            node_section* nsection = static_cast<node_section*>(nd);
            auto tag = str::repeat("%", nsection->level);
            print(tag);
            print(" ");
            auto line = list_first(&nsection->children);
            if (line != list_end(&nsection->children)) {
                assert(line->kind == NODE_KIND_LINE);
                print_tree(line, 0, print); // 0: 不需要加空白前缀
                print("\n");
                list_foreach_range(child, list_next(line), list_end(&nsection->children)) {
                    print_tree(child, ident + 1, print);
                }
                print("\n");
            }
        } break;
        case NODE_KIND_PARAGRAPH: {
            node_paragraph* nparagraph = static_cast<node_paragraph*>(nd);
            list_foreach(child, (&nparagraph->children)) {
                print_tree(child, ident, print);
            }
            print("\n");
        } break;
        case NODE_KIND_LINE: {
            print(str::make_spaces(ident * 4));
            node_line* nline = static_cast<node_line*>(nd);
            list_foreach(child, (&nline->children)) {
                print_tree(child, ident + 1, print);
            }
            print("\n");
        } break;
        case NODE_KIND_SEPARATOR: {
            print(str::make_spaces(ident * 4));
            node_separator* nseparator = static_cast<node_separator*>(nd);
            print(str::repeat(nseparator->separator, 4));
            print("\n");
        } break;
        case NODE_KIND_COMMENT: {
            print(str::make_spaces(ident * 4));
            node_comment* ncomment = static_cast<node_comment*>(nd);
            print("<!-- \n");
            for (auto& line : ncomment->lines) {
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
            node_param* nparam = static_cast<node_param*>(nd);
            print("@param ");
            for (size_t index = 0; index < nparam->names.size(); index++) {
                if (index != 0) {
                    print(", ");
                }
                print(nparam->names[index]);
            }
            print(": ");
            auto child = list_first(&nparam->children);
            if (child != list_end(&nparam->children)) {
                assert(child->kind == NODE_KIND_LINE); // 第一行必定是 line
                print_tree(child, 0, print);
                print("\n");
                // 其余的都是子节点
                for (child = list_next(child); child != list_end(&nparam->children); child = list_next(child)) {
                    print_tree(child, ident + 1, print);
                }
            }
            print("\n");
        } break;
        case NODE_KIND_RETURN: {
            print(str::make_spaces(ident * 4));
            print("@return: ");
            node_return* nreturn = static_cast<node_return*>(nd);
            auto child = list_first(&nreturn->children);
            if (child != list_end(&nreturn->children)) {
                assert(child->kind == NODE_KIND_LINE); // 第一行必定是 line
                print_tree(child, 0, print);
                print("\n");
                // 其余的都是子节点
                for (child = list_next(child); child != list_end(&nreturn->children); child = list_next(child)) {
                    print_tree(child, ident + 1, print);
                }
            }
            print("\n");
        } break;
        case NODE_KIND_BCODE: {
            print(str::make_spaces(ident * 4));
            node_bcode* nbcode = static_cast<node_bcode*>(nd);
            print("```");
            print(nbcode->language);
            if (!nbcode->properties.empty()) {
                print(": ");
                for (auto itr = nbcode->properties.begin(); itr != nbcode->properties.end(); itr++) {
                    if (itr != nbcode->properties.begin()) {
                        print(", ");
                    }
                    print(itr->first);
                    print("=");
                    print(itr->second);
                }
            }
            print("\n");

            list_foreach(child, &(nbcode->children)) {
                print_tree(child, ident, print);
            }

            print(str::make_spaces(ident * 4));
            print("```\n");
        } break;
        case NODE_KIND_EMBED: {
            node_embed* nembed = static_cast<node_embed*>(nd);
            list_foreach(child, &(nembed->children)) {
                print_tree(child, ident, print);
            }
        } break;
        case NODE_KIND_IFORMULA: {
            node_iformula* niformula = static_cast<node_iformula*>(nd);
            print("$");
            print(niformula->text);
            print("$");
        } break;
        case NODE_KIND_BFORMULA: {
            node_bformula* nbformula = static_cast<node_bformula*>(nd);
            print("$$\n");
            for (auto& line : nbformula->lines) {
                print(line);
                print("\n");
            }
            print("$$\n");
        } break;
        case NODE_KIND_LIST: {
            print(str::make_spaces(ident * 4));
            node_list* nlist = static_cast<node_list*>(nd);
            print(str::repeat("*", nlist->level));
            print(" ");
            auto first = list_first(&nlist->children);
            if (first != list_end(&nlist->children)) {
                print_tree(first, 0, print);
                print("\n");
                list_foreach_range(child, list_next(first), (&nlist->children)) {
                    print_tree(child, ident + 1, print);
                }
            }
        } break;
        case NODE_KIND_HLINK: {
            node_hlink* nhlink = static_cast<node_hlink*>(nd);
            print("[");
            print(nhlink->name);
            print("](");
            print(nhlink->url);
            print(")");
        } break;
        case NODE_KIND_IMAGE: {
            node_image* nimage = static_cast<node_image*>(nd);
            print("![");
            print(nimage->name);
            print("](");
            print(nimage->url);
            print(")");
        } break;
        case NODE_KIND_ANCHOR: {
        } break;
        case NODE_KIND_EMPHASIS: {
            // @[r]**dsdsdsd**
            node_emphasis* nstrong = static_cast<node_emphasis*>(nd);
            if (nstrong->types & emphasis_type::ET_BOLD) {
                print("**");
                print(nstrong->text);
                print("**");
            } else if (nstrong->types & emphasis_type::ET_STRIKETHROUGH) {
                print("~~");
                print(nstrong->text);
                print("~~");
            } else if (nstrong->types & emphasis_type::ET_ITALIC) {
                print("@/[");
                print(nstrong->text);
                print("]");
            } else if (nstrong->types & emphasis_type::ET_UNDERLINE) {
                print("@_[");
                print(nstrong->text);
                print("]");
            }
        } break;
        case NODE_KIND_TEXT: {
            node_text* ntext = static_cast<node_text*>(nd);
            print(ntext->text);
        } break;
        case NODE_KIND_ICODE: {
            node_icode* ntext = static_cast<node_icode*>(nd);
            print("`");
            print(ntext->text);
            print("`");
        } break;
        case NODE_KIND_ANNO: {
            node_anno* nanno = static_cast<node_anno*>(nd);
            print("@");
            print(nanno->tag);
            print("{");
            for (size_t index = 0; index < nanno->names.size(); index++) {
                if (index != 0) [[likely]] {
                    print(", ");
                }
                print(nanno->names[index]);
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

auto cmd_help(int argc, char* argv[]) -> int {
    constexpr std::string_view help_text = //
        "Usage:\n"                         //
        "   gendoc [InputFile] [-o OutputFile] [-r RootDirectory]\n";
    std::cout << help_text;

    return 0;
}

enum class parse_state : uint16_t {
    NORMAL,
    ENTER_PARAGRAPH,
    // ENTER_LIST,
};

struct line_type {
    line_type* next{nullptr};
    line_type* prev;

    size_t line_no{0};
    std::string line_text;

    explicit line_type() {
        list_head_init(this);
    };

    explicit line_type(size_t no, std::string_view text)
        : line_type() {
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

struct parse_context {
    explicit parse_context(FILE* ifile, const std::string& rd)
        : root_directory_{rd}
        , reader_{ifile} {
        project_ = new node_project;
        parent_ = project_;
    }

    ~parse_context() {
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

    inline auto root() -> node_project* {
        return project_;
    }

    inline auto root() const -> const node_project* {
        return project_;
    }

    inline auto detach_root() -> node_project* {
        auto ret = project_;
        project_ = nullptr;
        return ret;
    }

    // inline auto state() const -> parse_state {
    //     return state_;
    // }
    //
    // inline auto state(parse_state st) -> void {
    //     state_ = st;
    // }

private:
    const std::string& root_directory_;
    line_reader reader_;
    // FILE* writer_{nullptr};

    node_project* project_{nullptr};
    node_article* article_{nullptr};
    node* parent_{nullptr};

    size_t rpos_{0};
    // parse_state state_{parse_state::NORMAL};
};

auto node_traverse(node* root, const std::function<void(node* n)>& enter_proc, const std::function<void(node* n)>& leave_proc) -> void {
    assert(root != nullptr);
    enter_proc(root);
    for (auto item = list_first(&root->children); item != list_end(&root->children); item = list_next(item)) {
        node_traverse(item, enter_proc, leave_proc);
    }
    leave_proc(root);
}

struct sanitize_context {
public:
    explicit sanitize_context(const std::string& root_directory)
        : root_directory_{root_directory} {
    }

    auto haneld_embed(node_embed* embed) -> void {
        assert(embed != nullptr);
        const code_block* block = find_block(embed->file_name, embed->block_name);
        if (block == nullptr) {
            if (!load_block_file(root_directory_, embed->file_name)) {
                // TODO load block file failed
                return;
            }

            block = find_block(embed->file_name, embed->block_name);
            if (block == nullptr) {
                // TODO load block file failed
                return;
            }
        }

        assert(block != nullptr);
        for (auto& line : block->lines) {
            node_text* text = new node_text(line);
            embed->append(text);
        }
    }

    struct code_block {
        std::string block_name;
        std::vector<std::string> lines;
    };

    auto enter(node* n) -> void {
        assert(n != nullptr);
        switch (n->kind) {
            case NODE_KIND_IMAGE: {
                node_image* nimage = static_cast<node_image*>(n);
                std::string full_image_file = str::join_path({root_directory_, nimage->url});
                if (!std::filesystem::exists(full_image_file)) {
                    // TODO report error： file is not exist;
                }
            } break;
            case NODE_KIND_EMBED: {
                node_embed* nembed = static_cast<node_embed*>(n);
                haneld_embed(nembed);
            } break;
            default: {
            } break;
        }
    }

    auto leave(node* n) -> void {
        assert(n != nullptr);
    }

private:
    auto save_new_block(const std::string& block_file, const std::string& block_name, std::vector<std::string>& block_lines) -> void {
        code_block* new_block = new code_block;
        scope_guard guard_new_block([new_block] { delete new_block; });
        new_block->block_name = block_name;
        std::swap(new_block->lines, block_lines);

        // 文件如果是首次添加
        auto itr = all_code_blocks_.find(block_file);
        if (itr == all_code_blocks_.cend()) {
            std::map<std::string, code_block*> block_maping;
            auto add_result = all_code_blocks_.emplace(block_file, block_maping);
            assert(add_result.second);
            itr = add_result.first;
        }

        itr->second[block_name] = new_block;
        guard_new_block.dismiss();
    }

    auto load_block_file(const std::string& root_directory, const std::string& filepath) -> bool {
        // Return fail if file not exists
        std::string full_filepath = str::join_path(std::array{root_directory, filepath});
        if (!std::filesystem::exists(full_filepath)) {
            return false;
        }

        // If loaded, just skip
        auto itr = all_code_blocks_.find(filepath);
        if (itr != all_code_blocks_.cend()) {
            return true;
        }

        // Scan file and save all block
        bool enter_block = false;
        std::string curr_block_name;
        std::vector<std::string> block_lines;
        str::read_lines(full_filepath, false, //
            [&enter_block, &curr_block_name, &block_lines, &filepath, this](size_t lineno, std::string_view linetext) -> int {
                static std::regex pattern(R"(\s*//.*(@block|@end)\s+([a-zA-Z][a-zA-Z0-9_]*)\s*$)");
                svmatch match_results;
                bool matched = std::regex_match(linetext.begin(), linetext.end(), match_results, pattern);
                if (!matched) {
                    if (enter_block) {
                        block_lines.emplace_back(linetext);
                    }
                    return 0;
                }

                // Block begin
                if (!enter_block && (match_results[1] == "@block")) {
                    enter_block = true;
                    curr_block_name = match_results[2];
                    return 0;
                }

                // Block end
                if (enter_block && (match_results[1] == "@end")) {
                    enter_block = false;
                    save_new_block(filepath, curr_block_name, block_lines);
                    return 0;
                }

                return 0;
            });
        return true;
    }

    auto find_block(const std::string& block_file, const std::string& block_name) -> const code_block* {
        auto itr_file = all_code_blocks_.find(block_file);
        if (itr_file == all_code_blocks_.cend()) {
            return nullptr;
        }

        auto itr_block = itr_file->second.find(block_name);
        if (itr_block == itr_file->second.cend()) {
            return nullptr;
        }

        return itr_block->second;
    }

private:
    std::map<std::string, std::map<std::string, code_block*>> all_code_blocks_;
    std::string root_directory_;

    // std::list<node_image*> images;
    // std::list<node_hlink*> hlinks;
    // std::list<node_anno*> refs;
    // std::map<std::string, node_anchor*> anchors;
    //
    // // register anchors
    // inline auto register_anchor(node_anchor* anchor) -> void {
    //     assert(anchor != nullptr);
    //     for (const auto& name : anchor->names) {
    //         auto itr = std::as_const(anchors).find(name);
    //         if (itr != anchors.cend()) {
    //             // TODO need to print log：anchor redefined
    //             continue;
    //         }
    //         anchors.emplace(name, anchor);
    //     }
    // }
    //
    // // register unresolved refer
    // inline auto register_ref(node_anno* ref) -> void {
    //     assert(ref != nullptr);
    //     if ((ref->type != "ref") && (ref->type != "")) {
    //         // 出现这种情况，通常是代码有 bug 了
    //         assert(false);
    //         return;
    //     }
    //
    //     refs.emplace_back(ref);
    // }
    //
    // inline auto register_hlink(node_hlink* hlink) -> void {
    //     assert(hlink != nullptr);
    //     hlinks.emplace_back(hlink);
    // }
    //
    // inline auto register_image(node_image* image) -> void {
    //     assert(image != nullptr);
    //     images.emplace_back(image);
    // }
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

    auto remain() const -> std::string_view {
        return str::take_view(line_, rpos_);
    }

private:
    std::string_view line_;
    str::size_type start_{0};
    str::size_type rpos_{0};
    bool ok_{true};
};

auto try_parse_line_range(node* parent, std::string_view line, str::range_type range) -> void;

auto try_parse_chapter(parse_context& context, int8_t level, str::range_type range) -> void {
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

    try_parse_line_range(context.parent(), context.line_text(), range);

    context.pop();
}

// % xxx
auto try_parse_section(parse_context& context, int32_t level, str::range_type range) -> void {
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

    try_parse_line_range(context.parent(), context.line_text(), range);

    context.pop();
}

// * xxx
auto try_parse_list(parse_context& context, int8_t level, str::range_type range) -> void {
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
    try_parse_line_range(context.parent(), context.line_text(), range);
    context.pop();
}

auto try_parse_emphasis(std::string_view line, str::range_type& range) -> node_emphasis* {
    std::string_view range_text = str::take_view(line, range);
    acceptor acceptor(range_text);
    std::string_view content;

    // 未支持转义： **abc\***
    if (acceptor.from(0).accept("**").accept_until(content, "**")) {
        range = acceptor.range().shift(range.pos);
        return new node_emphasis(emphasis_type::ET_BOLD, content);
    }

    // 未支持转义： ~~abc\~~~
    if (acceptor.from(0).accept("~~").accept_until(content, "~~")) {
        range = acceptor.range().shift(range.pos);
        return new node_emphasis(emphasis_type::ET_STRIKETHROUGH, content);
    }

    // @#*/~_[xxxx]

    return nullptr;
}

// auto try_parse_em(std::string_view line, str::range_type& range) -> node_emphasis* {
//     acceptor acceptor(str::take_view(line, range));
//     std::string_view content;
//
//     // em
//     if (!acceptor.from(0).accept("*").accept_until(content, "*")) {
//         return nullptr;
//     }
//
//     range = acceptor.range().shift(range.pos);
//     return new node_emphasis(emphasis_type::ET_BOLD, content);
// }

auto try_parse_icode(std::string_view line, str::range_type& range) -> node_icode* {
    acceptor acceptor(str::take_view(line, range));
    std::string_view content;

    if (!acceptor.from(0).accept('`').accept_until(content, '`')) {
        return nullptr;
    }

    range = acceptor.range().shift(range.pos);
    node_icode* icode = new node_icode(content);
    return icode;
}

auto try_parse_hlink(std::string_view line, str::range_type& range) -> node_hlink* {
    acceptor acceptor(str::take_view(line, range));

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

auto try_parse_anno(std::string_view line, str::range_type& range) -> node_anno* {
    // @xxx{} @xxx{yyy, yyy}
    // @{yyy} @{yyy, yyy}
    // @xxx yyy, yyy:
    acceptor acceptor(str::take_view(line, range));
    static std::regex name_pattern{R"(#?[a-zA-z_][a-zA-z0-9_]*)"};
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

            acceptor.skip_n(1);
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

auto try_parse_image(std::string_view line, str::range_type& range) -> node_image* {
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

auto try_parse_color(std::string_view line, str::range_type& range) -> node_color* {
    // @#FF0000[xxx]
    // @#red[xxx]
    std::string_view remain = str::take_view(line, range);
    acceptor acceptor(str::take_view(remain, range));
    std::string_view color_num;
    if (!acceptor.from(0).accept("@#")) {
        return nullptr;
    }

    if (!acceptor.accept(color_num, std::regex("([a-zA-Z][a-zA-Z0-9]*|[0-9a-fA-F]{6})"))) {
        return nullptr;
    }

    std::string_view text;
    if (acceptor.accept('[').accept_until(text, '\\', ']')) {
        node_color* color = new node_color(color_num);
        color->append(new node_text(text)); // TODO 需要递归解析
        return color;
    }

    if (acceptor.accept(':')) {
        auto color_range = str::range(acceptor.pos(), remain.size() - acceptor.pos()).shift(range.pos);
        node_color* color = new node_color(color_num);
        color->append(new node_text(str::take_view(line, color_range))); // TODO 需要递归解析
        return color;
    }

    return nullptr;
}

auto try_parse_emphasis_scoped(std::string_view line, str::range_type& range) -> node_emphasis* {
    // @*[xxx]
    // @/[xxx]
    // @~[xxx]
    // @_[xxx]
    std::string_view remain = str::take_view(line, range);
    // static str::charset_type charset("*/~_");
    size_t pos = 1;
    uint8_t emphasis_mask = 0;
    while (pos < remain.size()) {
        switch (remain[pos]) {
            case '*':
                emphasis_mask |= ET_BOLD;
                break;
            case '/':
                emphasis_mask |= ET_ITALIC;
                break;
            case '~':
                emphasis_mask |= ET_STRIKETHROUGH;
                break;
            case '_':
                emphasis_mask |= ET_UNDERLINE;
                break;
            default:
                break;
        }

        if (remain[pos] != '[') {
            break;
        }
        pos++;
    }

    if (pos >= remain.size()) {
        // TODO 还未识别完
        return nullptr;
    }

    assert(remain[pos] != '[');
    pos++;

    auto result = str::accept_until(remain, pos, '\\', ']');
    if (!result) {
        // TODO 为找到结束符
        return nullptr;
    }

    return new node_emphasis(emphasis_mask, str::take_view(line, result.value().shift(range.begin())));
}

auto try_parse_line_range(node* parent, std::string_view line, str::range_type range) -> void {
    size_t rpos = range.begin();
    size_t curr = rpos;
    while (curr < range.end()) {
        char c = line[curr];
        str::range_type parse_range{curr, (range.end() - curr)};
        switch (c) {
            case '*':
                [[fallthrough]];
            case '~': {
                // **xxx** 或者 ~~xxx~~
                if (auto node = try_parse_emphasis(line, parse_range); node != nullptr) {
                    if (rpos < parse_range.begin()) {
                        node_text* text = new node_text(str::take_view(line, str::range(rpos, (parse_range.begin() - rpos))));
                        parent->append(text);
                    }

                    parent->append(node);
                    rpos = curr = parse_range.end();
                    continue;
                }
            } break;
            case '`': {
                if (auto node = try_parse_icode(line, parse_range); node != nullptr) {
                    if (rpos < parse_range.begin()) {
                        node_text* text = new node_text(str::take_view(line, str::range(rpos, (parse_range.begin() - rpos))));
                        parent->append(text);
                    }

                    parent->append(node);
                    rpos = curr = parse_range.end();
                    continue;
                }
            } break;
            case '[': {
                if (auto node = try_parse_hlink(line, parse_range); node != nullptr) {
                    if (rpos < parse_range.begin()) {
                        node_text* text = new node_text(str::take_view(line, str::range(rpos, (parse_range.begin() - rpos))));
                        parent->append(text);
                    }

                    parent->append(node);
                    rpos = curr = parse_range.end();
                    continue;
                }
            } break;
            case '@': {
                // @#xxx[yyy]
                // @*[yyy]
                // @~[yyy]
                // @/[yyy]
                // @-[yyy]
                if ((line.size() - curr) < 2) {
                    // TODO error 太短
                    continue;
                }

                char ch = line[curr + 1];
                switch (ch) {
                    case '#': {
                        node_color* color = try_parse_color(line, parse_range);
                        if (color != nullptr) {
                            parent->append(color);
                            rpos = curr = parse_range.end();
                        }
                    } break;
                    case '/':
                        [[fallthrough]];
                    case '*':
                        [[fallthrough]];
                    case '~':
                        [[fallthrough]];
                    case '_': {
                        node_emphasis* emphasis = try_parse_emphasis_scoped(line, parse_range);
                        if (emphasis != nullptr) {
                            parent->append(emphasis);
                            rpos = curr = parse_range.end();
                        }
                    } break;
                    default:
                        // TODO error: 不识别的符号
                        break;
                }

                if (auto node = try_parse_anno(line, parse_range); node != nullptr) {
                    if (rpos < parse_range.begin()) {
                        node_text* text = new node_text(str::take_view(line, str::range(rpos, (parse_range.begin() - rpos))));
                        parent->append(text);
                    }

                    parent->append(node);
                    rpos = curr = parse_range.end();
                    continue;
                }
            } break;
            case '!': {
                if (auto node = try_parse_image(line, parse_range); node != nullptr) {
                    if (rpos < parse_range.begin()) {
                        node_text* text = new node_text(str::take_view(line, str::range(rpos, (parse_range.begin() - rpos))));
                        parent->append(text);
                    }

                    parent->append(node);
                    rpos = curr = parse_range.end();
                    continue;
                }
            } break;
            default: {
                // // 扫描字符串行，知道遇到指定的分隔符
                // auto text_range = str::accept_until(line, curr, str::charset("*`[@!~"));
                // curr = (text_range ? text_range->end() : parse_range.end());
            } break;
        } // switch

        // 从下一个字符开始解析
        curr++;

        // 扫描字符串行，知道遇到指定的分隔符
        auto text_range = str::accept_until(line, curr, str::charset("*`[@!~"));
        curr = (text_range ? text_range->end() : parse_range.end());
    }

    // 这一行最后一部分
    if (rpos < curr) {
        node_text* text = new node_text(str::take_view(line, str::range(rpos, (curr - rpos))));
        parent->append(text);
    }
}

auto try_parse_line(parse_context& context) -> void {
    context.push(new node_line);

    try_parse_line_range(context.parent(), context.line_text(), str::range(0, context.line_text().size()));

    context.pop();
}

auto try_parse_commentblock(parse_context& context) -> void {
    node_comment* comment = new node_comment;
    context.append_child(comment);

    do {
        comment->lines.emplace_back(context.line_text());
        if (str::ends_with(context.line_text(), "-->")) {
            return;
        }
    } while (context.next_line());
}

auto try_parse_commentline(parse_context& context) -> void {
    node_comment* comment = new node_comment;
    comment->lines.emplace_back(context.line_text());
    context.append_child(comment);
}

auto try_parse_param(parse_context& context) -> void {
    const std::string& line = context.line_text();
    str::range_type range{0, line.size()};

    // 形式：@xxx yyy, yyy:
    acceptor acceptor(str::take_view(line, range));
    static std::regex name_pattern{R"([a-zA-Z_][a-zA-Z_0-9]*)"};
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
    context.push(new node_line());

    // 后面是一个行内的元素
    auto param_list_range = acceptor.range().shift(range.pos);
    try_parse_line_range(context.parent(), context.line_text(), str::range(param_list_range.end(), line.size() - param_list_range.end()));

    context.pop();
    context.pop();
}

auto try_parse_return(parse_context& context, str::range_type range) -> void {
    context.push(new node_return);
    context.push(new node_line);

    try_parse_line_range(context.parent(), context.line_text(), range);

    context.pop(); // pop line
    context.pop(); // pop return
}

auto try_parse_notice(parse_context& context, str::range_type range) -> void {
    context.push(new node_notice);
    context.push(new node_line);

    try_parse_line_range(context.parent(), context.line_text(), range);

    context.pop(); // pop line
    context.pop(); // pop notice
}

auto try_parse_embed(std::string_view line, str::range_type range) -> node_embed* {
    // @embed{yyy} "zzz"
    // @embed yyy: "zzz"
    // @embed: "zzz"
    acceptor acceptor(str::take_view(line, range));
    static std::regex name_pattern{R"([a-zA-Z][a-zA-Z0-9_]*)"};
    std::string_view block_name;

    // @embed{yyy} "zzz"
    if (acceptor.from(0).accept("@embed").accept('{').skip_spaces().accept(block_name, name_pattern).skip_spaces().accept('}')) {
        return new node_embed(str::trim_surrounding(block_name), str::trim_surrounding(acceptor.remain()));
    }

    // @embed yyy: "zzz"
    if (acceptor.from(0).accept("@embed").skip_spaces(1).accept(block_name, name_pattern).skip_spaces().accept(':')) {
        return new node_embed(str::trim_surrounding(block_name), str::trim_surrounding(acceptor.remain()));
    }

    // @embed: "zzz"
    if (acceptor.from(0).accept("@embed").skip_spaces().accept(':')) {
        return new node_embed(str::trim_surrounding(block_name), str::trim_surrounding(acceptor.remain()));
    }

    return nullptr;
}

auto try_parse_bcode(parse_context& context) -> void {
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

        if (str::starts_with(context.line_text(), "@embed")) {
            node_embed* embed = try_parse_embed(context.line_text(), str::range(context.line_text()));
            if (embed == nullptr) {
                assert(false); // TODO 识别 include 语法错误
                return;
            }
            bcode->append(embed);
        } else {
            bcode->append(new node_text(context.line_text()));
        }
    }
}

auto enter_paragraph(parse_state& state, parse_context& context) -> void {
    // paragraph-begin
    if (state == parse_state::NORMAL) {
        context.push(new node_paragraph());
        state = parse_state::ENTER_PARAGRAPH;
    }
}

auto leave_paragraph(parse_state& state, parse_context& context) -> void {
    // paragraph-end
    if (state != parse_state::NORMAL) {
        context.pop();
        state = parse_state::NORMAL;
    }
}

auto try_parse_chapterend(parse_context& context, int32_t level, str::range_type range) -> void {
    // auto level = matches.length(1);
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
}

auto try_parse_sectionend(parse_context& context, int32_t level, str::range_type range) -> void {
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
}

auto try_parse_listend(parse_context& context, int32_t level, str::range_type range) -> void {
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
}

auto try_parse_spacelines(parse_context& context) -> void {
    // 消费掉所有空行、空白行
    while (context.next_line()) {
        auto& line = context.line_text();
        if (!str::is_space_or_empty(line)) {
            // 回退当前行，编译借用外层循环的判断条件，实现统一接口退出
            context.rollback_line();
            break;
        }
    }
}

auto try_parse_separator(parse_context& context) -> void {
    const std::string& line = context.line_text();
    context.append_child(new node_separator(line[0]));
}

auto try_parse_article(parse_context& context) -> void {
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
            leave_paragraph(state, context);
            try_parse_spacelines(context);
            continue;
        }

        // 不同的分割线 .... ---- ====
        static std::regex separator_pattern{R"((\.{4,})|(-{4,})|(={4,}))"};
        if (std::regex_match(line.begin(), line.end(), matches, separator_pattern)) {
            leave_paragraph(state, context); // 立即打断当前block
            try_parse_separator(context);
            continue;
        }

        // @param 开头的行
        static std::regex param_pattern{R"(^(@param)(\s+|\s*:\s*).*)"};
        if (std::regex_match(line.begin(), line.end(), matches, param_pattern)) {
            leave_paragraph(state, context); // 立即打断当前block
            try_parse_param(context);
            continue;
        }

        // @return 开头的行
        static std::regex return_pattern{R"(^(@return)(\s+|\s*:\s*).*)"};
        if (std::regex_match(line.begin(), line.end(), matches, return_pattern)) {
            leave_paragraph(state, context); // 立即打断当前block
            auto prefix_len = matches.length(1) + matches.length(2);
            try_parse_return(context, str::range(prefix_len, (line.size() - prefix_len)));
            continue;
        }

        // @notice 开头的行
        static std::regex notice_pattern{R"(^(@notice)(\s+|\s*:\s*).*)"};
        if (std::regex_match(line.begin(), line.end(), matches, param_pattern)) {
            leave_paragraph(state, context); // 立即打断当前block
            auto prefix_len = matches.length(1) + matches.length(2);
            try_parse_notice(context, str::range(prefix_len, (line.size() - prefix_len)));
            continue;
        }

        // 代码块
        if (str::starts_with(line, "```")) {
            leave_paragraph(state, context); // 立即打断当前block
            try_parse_bcode(context);
            continue;
        }

        //! 注释块: <!-- ... -->
        if (str::starts_with_word(line, "<!--")) {
            leave_paragraph(state, context); // 立即打断当前block
            try_parse_commentblock(context);
            continue;
        }

        // 行注释
        if (str::starts_with_word(line, "//!")) {
            leave_paragraph(state, context); // 立即打断当前block
            try_parse_commentline(context);
            continue;
        }

        // 标题行：# 号开头的行
        static std::regex chapter_pattern{R"(^(#+)(\s+).*)"};
        if (std::regex_match(line.begin(), line.end(), matches, chapter_pattern)) {
            leave_paragraph(state, context); // 立即打断当前block
            auto level = matches.length(1);
            auto space_n = matches.length(2);
            try_parse_chapter(context, static_cast<int8_t>(level), str::range((level + space_n), line.size() - (level + space_n)));
            continue;
        }

        // 回退到特定标题级别
        static std::regex chapterend_pattern{R"(^(#+)(\s?)(<<)(\s.*)?$)"};
        if (std::regex_match(line.begin(), line.end(), matches, chapterend_pattern)) {
            leave_paragraph(state, context); // 立即打断当前block
            auto level = matches.length(1);
            auto space_n = matches.length(2);
            auto prefix_len = level + space_n + 2; // 2: length of '<<'
            try_parse_chapterend(context, level, str::range(prefix_len, line.size() - prefix_len));
            continue;
        }

        // 章节内分级： % 号开头的行
        static std::regex section_pattern{R"(^(%+)(\s+).*)"};
        if (std::regex_match(line.begin(), line.end(), matches, section_pattern)) {
            leave_paragraph(state, context); // 立即打断当前block
            auto level = matches.length(1);
            auto space_n = matches.length(2);
            try_parse_section(context, static_cast<int32_t>(level), str::range((level + space_n), line.size() - (level + space_n)));
            continue;
        }

        // 回退到特定标题级别
        static std::regex sectionend_pattern{R"(^(%+)(\s?)<<(\s.*)?$)"};
        if (std::regex_match(line.begin(), line.end(), matches, sectionend_pattern)) {
            leave_paragraph(state, context); // 立即打断当前block
            auto level = matches.length(1);
            auto space_n = matches.length(2);
            auto prefix_len = level + space_n + 2; // 2: length of '<<'
            try_parse_sectionend(context, level, str::range(prefix_len, (line.size() - prefix_len)));
            continue;
        }

        // 遇到列表行
        static std::regex list_pattern{R"(^(\*+)(\s+).*)"};
        if (std::regex_match(line.begin(), line.end(), matches, list_pattern)) {
            leave_paragraph(state, context);
            auto level = matches.length(1);
            auto space_n = matches.length(2);
            auto prefix_len = level + space_n;
            try_parse_list(context, static_cast<int8_t>(level), str::range(prefix_len, line.size() - prefix_len));
            continue;
        }

        // 回退到特定标题级别
        static std::regex back_list_pattern{R"(^(\*+)(\s?)(<<)(\s.*)?$)"};
        if (std::regex_match(line.begin(), line.end(), matches, back_list_pattern)) {
            leave_paragraph(state, context); // 立即打断当前block
            auto level = matches.length(1);
            auto space_n = matches.length(2);
            auto prefix_len = level + space_n + 2; // 2: length of '<<'
            try_parse_listend(context, level, str::range(prefix_len, (line.size() - prefix_len)));
            continue;
        }

        // // 附加
        // static std::regex addition_pattern(R"(^+>)");
        // if (std::regex_match(line.begin(), line.end(), matches, addition_pattern)) {
        //     leave_paragraph(state, context); // 立即打断当前block
        //     try_parse_addition(context, level, str::range(prefix_len, (line.size() - prefix_len)));
        //     continue;
        // }

        // // 附加结束
        // static std::regex additionend_pattern(R"(^+<<)");
        // if (std::regex_match(line.begin(), line.end(), matches, addition_pattern)) {
        //     leave_paragraph(state, context); // 立即打断当前block
        //     try_parse_additionend(context, level, str::range(prefix_len, (line.size() - prefix_len)));
        //     continue;
        // }

        // 可能需要进入block
        enter_paragraph(state, context);

        // 处理本行（父级一定是block，行必须位于block内才有意义）
        assert(state == parse_state::ENTER_PARAGRAPH);
        assert(context.parent()->kind == NODE_KIND_PARAGRAPH);
        try_parse_line(context);
    }
}

auto try_parse_file(parse_context& context) -> std::string {
    // 重置 parent
    context.parent(context.root());

    try_parse_article(context);

    return {};
}

auto encode_html_text(std::string_view text) -> std::string {
    // #define HTML_ESCAPE_CHAR_TABLE()        \
    //     DEF_HTML_ESCAPE_CHAR('>', "&gt;")   \
    //     DEF_HTML_ESCAPE_CHAR('<', "&lt;")   \
    //     DEF_HTML_ESCAPE_CHAR('&', "&amp;")  \
    //     DEF_HTML_ESCAPE_CHAR('"', "&quot;") \
    //     DEF_HTML_ESCAPE_CHAR('\'', "&apos;") \
    //     /* (end) */

    // #define HTML_ESCAPE_U8CHAR_TABLE()   \
    //     DEF_HTML_ESCAPE_U8CHAR("&nbsp;") \
    //     DEF_HTML_ESCAPE_U8CHAR("&copy;") \
    //     DEF_HTML_ESCAPE_U8CHAR("&reg;")  \
    //     DEF_HTML_ESCAPE_U8CHAR("&euro;") \
    //     DEF_HTML_ESCAPE_U8CHAR("&bull;") \
    //     /* (end) */

    std::string result;
    for (size_t index = 0; index < text.size(); index++) {
        std::string_view::value_type ch = text[index];
        switch (ch) {
            case '>': {
                result.append("&gt;");
            } break;
            case '<': {
                result.append("&lt;");
            } break;
            case '&': {
                result.append("&amp;");
            } break;
            case '"': {
                result.append("&quot;");
            } break;
            case '\'': {
                result.append("&apos;");
            } break;
            default: {
                result.append(1, ch);
            } break;
        }
    }

    return result;
}

auto print_html(node* nd, const std::function<void(std::string_view)>& print) -> void {
    assert((nd != nullptr) && (print != nullptr));
    switch (nd->kind) {
        case NODE_KIND_PROJECT: {
            node_project* nproject = static_cast<node_project*>(nd);
            print("<!DOCTYPE html>\n");
            print("<html lang=\"en\">\n");
            print("<head>\n");
            print("<meta charset=\"UTF-8\">\n");
            print("<title>HTML5 示例</title>\n");
            print("<link rel=\"stylesheet\" href=\"cppreference.css\">\n");
            print("</head>\n");
            print("<body>\n");
            list_foreach(child, &(nproject->children)) {
                print_html(child, print);
            }
            print("</body>\n");
            print("</html>\n");
        } break;
        case NODE_KIND_PROJECTEND: {
            // const node_projectend* node = static_cast<const node_projectend*>(nd);
        } break;
        case NODE_KIND_ARTICLE: {
            node_article* narticle = static_cast<node_article*>(nd);
            print("<main>\n");
            list_foreach(child, &(narticle->children)) {
                print_html(child, print);
            }
            print("</main>\n");
        } break;
        case NODE_KIND_ARTICLEEND: {
            // const node_articlend* node = static_cast<const node_articlend*>(nd);
        } break;
        case NODE_KIND_CHAPTER: {
            node_chapter* nchapter = static_cast<node_chapter*>(nd);
            // <section class="chapter-?">
            print("<section class=\"chapter-");
            print(all_levels[nchapter->level - 1]);
            print("\">\n");

            // <h? class="chapter-?-head">
            print("<");
            print(all_headers[nchapter->level - 1]);
            print(">\n");
            node* first = list_first(&(nchapter->children));
            print_html(first, print);
            print("</");
            print(all_headers[nchapter->level - 1]);
            print(">\n");

            // <div class="chapter-?-body">
            print("<div class=\"chapter-");
            print(all_levels[nchapter->level - 1]);
            print("-body\">\n");

            list_foreach_range(child, list_next(first), list_end(&(nchapter->children))) {
                print_html(child, print);
            }

            // </div>
            print("</div>\n");

            // </section>
            print("</section>\n");
        } break;
        case NODE_KIND_CHAPTEREND: {
            // const node_chapterend* node = static_cast<const node_chapterend*>(nd);
        } break;
        case NODE_KIND_SECTION: {
            node_section* nsection = static_cast<node_section*>(nd);

            // <section class="section-?">
            print("<section class=\"section-");
            print(all_levels[nsection->level - 1]);
            print("\">\n");

            // <h? class="section-?-head">
            print("<");
            print(all_headers[nsection->level - 1]);
            print(" class=\"section-");
            print(all_levels[nsection->level - 1]);
            print("-head\">\n");
            node* first = list_first(&(nsection->children));
            print_html(first, print);

            // </h?>
            print("</");
            print(all_headers[nsection->level - 1]);
            print(">\n");

            // <div class="section-?-body">
            print("<div class=\"section-");
            print(all_levels[nsection->level - 1]);
            print("-body\">\n");

            list_foreach_range(child, list_next(first), list_end(&(nsection->children))) {
                print_html(child, print);
            }

            // </div>
            print("</div>\n");

            // </section>
            print("</section>\n");
        } break;
        case NODE_KIND_SECTIONEND: {
            // const node_sectionend* node = static_cast<const node_sectionend*>(nd);
        } break;
        case NODE_KIND_PARAM: {
            node_param* nparam = static_cast<node_param*>(nd);
            if ((list_prev(nparam) == list_end(&nd->parent->children)) || (nparam->prev->kind != NODE_KIND_PARAM)) {
                print("<table class=\"table-param\">\n");
            }

            print("<tr class=\"tr-param\">");

            print("<td class=\"td-param-names\">");
            for (size_t index = 0; index < nparam->names.size(); index++) {
                if (index != 0) {
                    print(", ");
                }
                print(encode_html_text(nparam->names[index]));
            }
            print("</td>");

            print("<td class=\"td-param-sep\">");
            print("-");
            print("</td>");

            print("<td class=\"td-param-desc\">");
            list_foreach(item, &nparam->children) {
                print_html(item, print);
            }
            print("</td>");

            print("</tr>\n");
            if ((list_next(nparam) == list_end(&nd->parent->children)) || (nparam->next->kind != NODE_KIND_PARAM)) {
                print("</table>\n");
            }
        } break;
        case NODE_KIND_RETURN: {
            node_return* nreturn = static_cast<node_return*>(nd);
            print("<p class=\"return\">");
            list_foreach(item, &nreturn->children) {
                print_html(item, print);
            }
            print("</p>\n");
        } break;
        case NODE_KIND_NOTICE: {
            node_notice* nnotice = static_cast<node_notice*>(nd);
            print("<p class=\"notice\">");
            list_foreach(item, &nnotice->children) {
                print_html(item, print);
            }
            print("</p>\n");
        } break;
        case NODE_KIND_LIST: {
            node_list* nlist = static_cast<node_list*>(nd);
            if ((list_prev(nlist) == list_end(&nd->parent->children)) || (nlist->prev->kind != NODE_KIND_LIST)) {
                print("<ul>\n");
            }
            print("<li>\n");
            list_foreach(child, &(nlist->children)) {
                print_html(child, print);
            }
            print("</li>\n");
            if (list_next(nlist) == list_end(&nd->parent->children) || (nlist->next->kind != NODE_KIND_LIST)) {
                print("</ul>\n");
            }
        } break;
        case NODE_KIND_PARAGRAPH: {
            node_paragraph* nparagraph = static_cast<node_paragraph*>(nd);
            print("<p>\n");
            list_foreach(child, &(nparagraph->children)) {
                print_html(child, print);
            }
            print("</p>\n");
        } break;
        case NODE_KIND_BCODE: {
            node_bcode* nbcode = static_cast<node_bcode*>(nd);
            print("<pre  class=\"source\">\n");
            list_foreach(child, &(nbcode->children)) {
                print_html(child, print);
            }
            print("</pre>\n");
        } break;
        case NODE_KIND_SEPARATOR: {
            node_separator* nseparator = static_cast<node_separator*>(nd);
            print(R"(<div style="border-top: 1px solid black; margin: 10px 0;"></div>)");
            print("\n");
        } break;
        case NODE_KIND_BFORMULA: {
            // const node_bformula* nbformula = static_cast<const node_bformula*>(nd);
        } break;
        case NODE_KIND_COMMENT: {
            node_comment* ncomment = static_cast<node_comment*>(nd);
            print("<!--\n");
            list_foreach(child, &(ncomment->children)) {
                print_html(child, print);
            }
            print("-->\n");
        } break;
        case NODE_KIND_IFORMULA: {
            // const node_iformula* niformula = static_cast<const node_iformula*>(nd);
        } break;
        case NODE_KIND_HLINK: {
            const node_hlink* nhlink = static_cast<const node_hlink*>(nd);
            print("<a href=\"");
            print(nhlink->url);
            print("\">");
            print(encode_html_text(nhlink->name));
            print("</a>");
        } break;
        case NODE_KIND_IMAGE: {
            const node_image* nimage = static_cast<const node_image*>(nd);
            print("<img src=\"");
            print(nimage->url);
            print("\" alt=\"");
            print(encode_html_text(nimage->name));
            print("\">");
        } break;
        case NODE_KIND_ANCHOR: {
            const node_anchor* nanchor = static_cast<const node_anchor*>(nd);
            for (auto& name : nanchor->names) {
                print("<a name=\"");
                print(encode_html_text(name));
                print("\"/>");
            }
        } break;
        case NODE_KIND_EMBED: {
            node_embed* nembed = static_cast<node_embed*>(nd);
            list_foreach(child, &(nembed->children)) {
                print_html(child, print);
                print("\n");
            }
        } break;
        case NODE_KIND_EMPHASIS: {
            // const node_emphasis* nemphasis = static_cast<const node_emphasis*>(nd);
        } break;
        case NODE_KIND_TEXT: {
            const node_text* ntext = static_cast<const node_text*>(nd);
            print(encode_html_text(ntext->text));
        } break;
        case NODE_KIND_ICODE: {
            const node_icode* nicode = static_cast<const node_icode*>(nd);
            print("<code class=\"source\">");
            print(encode_html_text(nicode->text));
            print("</code>");
        } break;
        case NODE_KIND_ANNO: {
            node_anno* nanno = static_cast<node_anno*>(nd);
            if ((nanno->tag == "ref") || nanno->tag.empty()) {
                for (size_t index = 0; index < nanno->names.size(); index++) {
                    if (index != 0) {
                        print(", ");
                    }
                    auto& name = nanno->names[index];
                    print("<a href=\"");
                    print(encode_html_text(name));
                    print("\">");
                    print(encode_html_text(str::remove_prefix_view(name, "#")));
                    print("</a>");
                }
            }
        } break;
        case NODE_KIND_COLOR: {
            // const node_color* node = static_cast<const node_color*>(nd);
        } break;
        case NODE_KIND_LINE: {
            node_line* node = static_cast<node_line*>(nd);
            list_foreach(child, &(node->children)) {
                print_html(child, print);
            }
            print("\n");
        } break;
        case NODE_KIND_TABLE: {
            // const node_table* node = reinterpret_cast<const node_table*>(nd);
        } break;
        case NODE_KIND_THEAD: {
            // const node_thead* node = static_cast<const node_thead*>(nd);
        } break;
        case NODE_KIND_TROW: {
            // const node_trow* node = static_cast<const node_trow*>(nd);
        } break;
        case NODE_KIND_TCOL: {
            // const node_tcol* node = static_cast<const node_tcol*>(nd);
        } break;
        default: {
        } break;
    }
}

} // namespace gendoc

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

auto main(int argc, char* argv[]) -> int {
    gendoc_options opts;
    std::string error = opts.load(argc, argv);
    if (!error.empty()) {
        std::cerr << error << std::endl;
        return 1;
    }

    if (!opts.output_file.empty()) {
        std::string output_directory = str::dirname(opts.output_file);
        if (!std::filesystem::exists(output_directory)) {
            std::error_code sys_error;
            if (!std::filesystem::create_directories(output_directory, sys_error)) {
                std::cerr << "Can not create output directory `" << output_directory << "'" << std::endl;
                return 1;
            }
        }
    }

    if (!std::filesystem::exists(opts.root_directory)) {
        std::cerr << "The project root directory is not exist `" << opts.root_directory << "'" << std::endl;
        return 1;
    }

    gendoc::node_project* project = nullptr;
    FILE* input_repl = ((opts.input_file.empty()) ? stdin : nullptr);
    str::with_file(opts.input_file, "r", input_repl, [&opts, &error, &project](FILE* ifile) -> void {
        assert(ifile != nullptr);
        gendoc::parse_context context{ifile, opts.root_directory};
        error = try_parse_file(context);
        if (!error.empty()) {
            return;
        }

        project = context.detach_root();
    });

    if (!error.empty()) {
        assert(project == nullptr);
        std::cerr << "Generate document failed: " << error << std::endl;
        return 1;
    }

    // 矫正和整理
    assert(project != nullptr);
    gendoc::sanitize_context context(opts.root_directory);
    gendoc::node_traverse(project, [&context](gendoc::node* nd) -> void { context.enter(nd); }, [&context](gendoc::node* nd) -> void { context.leave(nd); });

    // // 输出
    // gendoc::print_tree(project, 0, [](std::string_view text) {
    //     std::cout << text;
    //     std::cout.flush();
    // });

    // 输出
    FILE* output_repl = ((opts.output_file.empty()) ? stdout : nullptr);
    str::with_file(opts.output_file, "w+", output_repl, [project](FILE* ofile) -> void {
        gendoc::print_html(project, [ofile](std::string_view text) {
            fwrite(text.data(), 1, text.size(), ofile);
            fflush(ofile);
        });
    });

    return 0;
}