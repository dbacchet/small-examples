workspace(name = "vmath_examples")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")


http_archive(
    name = "googletest",
    urls = ["https://github.com/google/googletest/archive/release-1.10.0.tar.gz"],
    sha256 = "9dc9157a9a1551ec7a7e43daea9a694a0bb5fb8bec81235d8a1e6ef64c716dcb",
    strip_prefix = "googletest-release-1.10.0",
)

git_repository(
    name = "vmath",
    remote = "https://github.com/dbacchet/vmath.git",
    commit = "d47ecd3febbea1f81ae00c39aff0915883dd2080",
    shallow_since = "1586049986 -0700"
)
