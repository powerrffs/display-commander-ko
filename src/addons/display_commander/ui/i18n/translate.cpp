#include "translate.hpp"

#include <atomic>
#include <cstddef>
#include <cstring>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace display_commander::ui::i18n {
namespace {

struct Entry {
    const char* en;
    const char* ko;
};

constexpr Entry kKoreanTable[] = {
#include "ko_strings_table.hpp"
};

using Table = std::unordered_map<std::string_view, const char*>;

const Table& GetTable() {
    static const Table table = [] {
        Table t;
        t.reserve(sizeof(kKoreanTable) / sizeof(kKoreanTable[0]));
        for (const Entry& e : kKoreanTable) {
            t.emplace(std::string_view(e.en), e.ko);
        }
        return t;
    }();
    return table;
}

struct TransparentHash {
    using is_transparent = void;
    size_t operator()(std::string_view s) const noexcept { return std::hash<std::string_view>{}(s); }
};

// Composed results (icon prefix / "##" / "###" suffix). Only translated strings are cached, so the size is bounded by
// the table. Per thread: the overlay draws on one thread, and this avoids locking.
using Cache = std::unordered_map<std::string, std::string, TransparentHash, std::equal_to<>>;

std::atomic<bool> g_enabled{true};

const char* Lookup(std::string_view s) {
    const Table& t = GetTable();
    auto it = t.find(s);
    return it != t.end() ? it->second : nullptr;
}

// Leading ForkAwesome icon: one UTF-8 code point in the private use area U+E000..U+F8FF (EE xx xx / EF 80..A3 xx).
size_t IconPrefixLength(std::string_view s) {
    if (s.size() >= 3) {
        const auto b0 = static_cast<unsigned char>(s[0]);
        const auto b1 = static_cast<unsigned char>(s[1]);
        if (b0 == 0xEE || (b0 == 0xEF && b1 <= 0xA3)) {
            return 3;
        }
    }
    return 0;
}

// Translate visible text, trying "<icon><text>" as a whole first and then just "<text>".
// Returns true and fills out when a translation exists.
bool TranslateVisible(std::string_view visible, std::string& out) {
    if (const char* ko = Lookup(visible)) {
        out.assign(ko);
        return true;
    }
    const size_t icon = IconPrefixLength(visible);
    if (icon != 0) {
        if (const char* ko = Lookup(visible.substr(icon))) {
            out.assign(visible.substr(0, icon));
            out.append(ko);
            return true;
        }
    }
    return false;
}

}  // namespace

bool IsEnabled() { return g_enabled.load(std::memory_order_relaxed); }
void SetEnabled(bool enabled) { g_enabled.store(enabled, std::memory_order_relaxed); }

const char* TrText(const char* text) {
    if (text == nullptr || text[0] == '\0' || !IsEnabled()) {
        return text;
    }
    const std::string_view s(text);
    if (const char* ko = Lookup(s)) {
        return ko;  // fast path: static storage
    }
    if (IconPrefixLength(s) == 0) {
        return text;
    }
    thread_local Cache cache;
    if (auto it = cache.find(s); it != cache.end()) {
        return it->second.c_str();
    }
    std::string translated;
    if (!TranslateVisible(s, translated)) {
        return text;
    }
    return cache.emplace(std::string(s), std::move(translated)).first->second.c_str();
}

const char* TrLabel(const char* label) {
    if (label == nullptr || label[0] == '\0' || !IsEnabled()) {
        return label;
    }
    const std::string_view s(label);
    // Hidden labels ("##id" only) have nothing to translate.
    if (s.size() >= 2 && s[0] == '#' && s[1] == '#') {
        return label;
    }
    thread_local Cache cache;
    if (auto it = cache.find(s); it != cache.end()) {
        return it->second.c_str();
    }
    std::string result;
    const size_t id_pos = s.find("##");
    if (id_pos != std::string_view::npos) {
        // Whole label (e.g. "SR Preset##MainTabSRPreset") may be in the table with its id; else translate the
        // visible part and keep the original id suffix.
        if (const char* ko = Lookup(s)) {
            result.assign(ko);
        } else if (TranslateVisible(s.substr(0, id_pos), result)) {
            result.append(s.substr(id_pos));
        } else {
            return label;
        }
    } else {
        if (!TranslateVisible(s, result)) {
            return label;
        }
        // Keep the ImGui ID tied to the English label so two labels sharing a translation can't collide.
        result.append("###");
        result.append(s);
    }
    return cache.emplace(std::string(s), std::move(result)).first->second.c_str();
}

}  // namespace display_commander::ui::i18n
