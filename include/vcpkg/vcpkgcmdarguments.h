#pragma once

#include <vcpkg/base/fwd/system.process.h>

#include <vcpkg/fwd/vcpkgcmdarguments.h>
#include <vcpkg/fwd/vcpkgpaths.h>

#include <vcpkg/base/files.h>
#include <vcpkg/base/optional.h>
#include <vcpkg/base/span.h>
#include <vcpkg/base/stringview.h>

#include <map>
#include <memory>
#include <set>
#include <vector>

namespace vcpkg
{
    struct ParsedArguments
    {
        std::set<std::string, std::less<>> switches;
        std::map<std::string, std::string, std::less<>> settings;
        std::map<std::string, std::vector<std::string>, std::less<>> multisettings;

        const std::string* read_setting(StringLiteral setting) const noexcept;
    };

    struct CommandSwitch
    {
        constexpr CommandSwitch(const StringLiteral& name, const StringLiteral& short_help_text)
            : name(name), short_help_text(short_help_text)
        {
        }

        StringLiteral name;
        StringLiteral short_help_text;
    };

    struct CommandSetting
    {
        constexpr CommandSetting(const StringLiteral& name, const StringLiteral& short_help_text)
            : name(name), short_help_text(short_help_text)
        {
        }

        StringLiteral name;
        StringLiteral short_help_text;
    };

    struct CommandMultiSetting
    {
        constexpr CommandMultiSetting(const StringLiteral& name, const StringLiteral& short_help_text)
            : name(name), short_help_text(short_help_text)
        {
        }

        StringLiteral name;
        StringLiteral short_help_text;
    };

    struct CommandOptionsStructure
    {
        Span<const CommandSwitch> switches;
        Span<const CommandSetting> settings;
        Span<const CommandMultiSetting> multisettings;
    };

    struct CommandStructure
    {
        std::string example_text;

        size_t minimum_arity;
        size_t maximum_arity;

        CommandOptionsStructure options;

        std::vector<std::string> (*valid_arguments)(const VcpkgPaths& paths);
    };

    void print_usage();
    void print_usage(const CommandStructure& command_structure);

#if defined(_WIN32)
    using CommandLineCharType = wchar_t;
#else
    using CommandLineCharType = char;
#endif

    std::string create_example_string(const std::string& command_and_arguments);

    struct HelpTableFormatter
    {
        void format(StringView col1, StringView col2);
        void example(StringView example_text);
        void header(StringView name);
        void blank();
        void text(StringView text, int indent = 0);

        std::string m_str;
    };

    struct FeatureFlagSettings
    {
        bool registries;
        bool compiler_tracking;
        bool binary_caching;
        bool versions;
        bool manifests;
    };

    struct VcpkgCmdArguments
    {
        static VcpkgCmdArguments create_from_command_line(const Filesystem& fs,
                                                          const int argc,
                                                          const CommandLineCharType* const* const argv);
        static VcpkgCmdArguments create_from_arg_sequence(const std::string* arg_begin, const std::string* arg_end);

        static void append_common_options(HelpTableFormatter& target);

        constexpr static StringLiteral VCPKG_ROOT_DIR_ENV = "VCPKG_ROOT";
        constexpr static StringLiteral VCPKG_ROOT_DIR_ARG = "vcpkg-root";
        std::unique_ptr<std::string> vcpkg_root_dir;
        constexpr static StringLiteral MANIFEST_ROOT_DIR_ARG = "x-manifest-root";
        std::unique_ptr<std::string> manifest_root_dir;

        constexpr static StringLiteral BUILDTREES_ROOT_DIR_ARG = "x-buildtrees-root";
        std::unique_ptr<std::string> buildtrees_root_dir;
        constexpr static StringLiteral DOWNLOADS_ROOT_DIR_ENV = "VCPKG_DOWNLOADS";
        constexpr static StringLiteral DOWNLOADS_ROOT_DIR_ARG = "downloads-root";
        std::unique_ptr<std::string> downloads_root_dir;
        constexpr static StringLiteral INSTALL_ROOT_DIR_ARG = "x-install-root";
        std::unique_ptr<std::string> install_root_dir;
        constexpr static StringLiteral PACKAGES_ROOT_DIR_ARG = "x-packages-root";
        std::unique_ptr<std::string> packages_root_dir;
        constexpr static StringLiteral SCRIPTS_ROOT_DIR_ARG = "x-scripts-root";
        std::unique_ptr<std::string> scripts_root_dir;
        constexpr static StringLiteral BUILTIN_PORTS_ROOT_DIR_ARG = "x-builtin-ports-root";
        std::unique_ptr<std::string> builtin_ports_root_dir;
        constexpr static StringLiteral BUILTIN_REGISTRY_VERSIONS_DIR_ARG = "x-builtin-registry-versions-dir";
        std::unique_ptr<std::string> builtin_registry_versions_dir;
        constexpr static StringLiteral REGISTRIES_CACHE_DIR_ENV = "X_VCPKG_REGISTRIES_CACHE";
        constexpr static StringLiteral REGISTRIES_CACHE_DIR_ARG = "x-registries-cache";
        std::unique_ptr<std::string> registries_cache_dir;

        constexpr static StringLiteral DEFAULT_VISUAL_STUDIO_PATH_ENV = "VCPKG_VISUAL_STUDIO_PATH";
        std::unique_ptr<std::string> default_visual_studio_path;

        constexpr static StringLiteral TRIPLET_ENV = "VCPKG_DEFAULT_TRIPLET";
        constexpr static StringLiteral TRIPLET_ARG = "triplet";
        std::unique_ptr<std::string> triplet;
        constexpr static StringLiteral HOST_TRIPLET_ENV = "VCPKG_DEFAULT_HOST_TRIPLET";
        constexpr static StringLiteral HOST_TRIPLET_ARG = "host-triplet";
        std::unique_ptr<std::string> host_triplet;
        constexpr static StringLiteral OVERLAY_PORTS_ENV = "VCPKG_OVERLAY_PORTS";
        constexpr static StringLiteral OVERLAY_PORTS_ARG = "overlay-ports";
        std::vector<std::string> overlay_ports;
        constexpr static StringLiteral OVERLAY_TRIPLETS_ENV = "VCPKG_OVERLAY_TRIPLETS";
        constexpr static StringLiteral OVERLAY_TRIPLETS_ARG = "overlay-triplets";
        std::vector<std::string> overlay_triplets;

        constexpr static StringLiteral BINARY_SOURCES_ARG = "binarysource";
        std::vector<std::string> binary_sources;

        constexpr static StringLiteral CMAKE_SCRIPT_ARG = "x-cmake-args";
        std::vector<std::string> cmake_args;

        constexpr static StringLiteral EXACT_ABI_TOOLS_VERSIONS_SWITCH = "x-abi-tools-use-exact-versions";
        Optional<bool> exact_abi_tools_versions;

        constexpr static StringLiteral DEBUG_SWITCH = "debug";
        Optional<bool> debug = nullopt;
        constexpr static StringLiteral DEBUG_ENV_SWITCH = "debug-env";
        Optional<bool> debug_env = nullopt;
        constexpr static StringLiteral SEND_METRICS_SWITCH = "sendmetrics";
        Optional<bool> send_metrics = nullopt;
        // fully disable metrics -- both printing and sending
        constexpr static StringLiteral DISABLE_METRICS_ENV = "VCPKG_DISABLE_METRICS";
        constexpr static StringLiteral DISABLE_METRICS_SWITCH = "disable-metrics";
        Optional<bool> disable_metrics = nullopt;
        constexpr static StringLiteral PRINT_METRICS_SWITCH = "printmetrics";
        Optional<bool> print_metrics = nullopt;

        constexpr static StringLiteral WAIT_FOR_LOCK_SWITCH = "x-wait-for-lock";
        Optional<bool> wait_for_lock = nullopt;

        constexpr static StringLiteral IGNORE_LOCK_FAILURES_SWITCH = "x-ignore-lock-failures";
        constexpr static StringLiteral IGNORE_LOCK_FAILURES_ENV = "X_VCPKG_IGNORE_LOCK_FAILURES";
        Optional<bool> ignore_lock_failures = nullopt;

        constexpr static StringLiteral JSON_SWITCH = "x-json";
        Optional<bool> json = nullopt;

        constexpr static StringLiteral ASSET_SOURCES_ENV = "X_VCPKG_ASSET_SOURCES";
        constexpr static StringLiteral ASSET_SOURCES_ARG = "x-asset-sources";

        // feature flags
        constexpr static StringLiteral FEATURE_FLAGS_ENV = "VCPKG_FEATURE_FLAGS";
        constexpr static StringLiteral FEATURE_FLAGS_ARG = "feature-flags";

        constexpr static StringLiteral FEATURE_PACKAGES_SWITCH = "featurepackages";
        Optional<bool> feature_packages = nullopt;
        constexpr static StringLiteral BINARY_CACHING_FEATURE = "binarycaching";
        constexpr static StringLiteral BINARY_CACHING_SWITCH = "binarycaching";
        Optional<bool> binary_caching = nullopt;
        constexpr static StringLiteral COMPILER_TRACKING_FEATURE = "compilertracking";
        Optional<bool> compiler_tracking = nullopt;
        constexpr static StringLiteral MANIFEST_MODE_FEATURE = "manifests";
        Optional<bool> manifest_mode = nullopt;
        constexpr static StringLiteral REGISTRIES_FEATURE = "registries";
        Optional<bool> registries_feature = nullopt;
        constexpr static StringLiteral VERSIONS_FEATURE = "versions";
        Optional<bool> versions_feature = nullopt;

        constexpr static StringLiteral RECURSIVE_DATA_ENV = "X_VCPKG_RECURSIVE_DATA";

        bool binary_caching_enabled() const { return binary_caching.value_or(true); }
        bool compiler_tracking_enabled() const { return compiler_tracking.value_or(true); }
        bool registries_enabled() const { return registries_feature.value_or(true); }
        bool versions_enabled() const { return versions_feature.value_or(true); }
        bool manifests_enabled() const { return manifest_mode.value_or(true); }
        FeatureFlagSettings feature_flag_settings() const
        {
            FeatureFlagSettings f;
            f.binary_caching = binary_caching_enabled();
            f.compiler_tracking = compiler_tracking_enabled();
            f.registries = registries_enabled();
            f.versions = versions_enabled();
            f.manifests = manifests_enabled();
            return f;
        }

        bool output_json() const { return json.value_or(false); }

        std::string command;
        std::vector<std::string> command_arguments;

        ParsedArguments parse_arguments(const CommandStructure& command_structure) const;

        void imbue_from_environment();
        void imbue_from_fake_environment(const std::map<std::string, std::string, std::less<>>& env);

        // Applies recursive settings from the environment or sets a global environment variable
        // to be consumed by subprocesses; may only be called once per process.
        static void imbue_or_apply_process_recursion(VcpkgCmdArguments& args);

        void check_feature_flag_consistency() const;

        void debug_print_feature_flags() const;
        void track_feature_flag_metrics() const;

        Optional<std::string> asset_sources_template() const;

        const std::vector<std::string>& get_forwardable_arguments() const noexcept;

    private:
        void imbue_from_environment_impl(std::function<Optional<std::string>(ZStringView)> get_env);

        Optional<std::string> asset_sources_template_env;        // for ASSET_SOURCES_ENV
        std::unique_ptr<std::string> asset_sources_template_arg; // for ASSET_SOURCES_ARG

        std::set<std::string, std::less<>> command_switches;
        std::map<std::string, std::vector<std::string>, std::less<>> command_options;

        std::vector<std::string> forwardable_arguments;
    };
}
