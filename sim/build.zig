const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const lib = b.addStaticLibrary(.{
        .name = "kirby_sim",
        .root_source_file = .{ .path = "src/kirby_sim.zig" },
        .target = target,
        .optimize = optimize,
    });

    b.installArtifact(lib);
}
