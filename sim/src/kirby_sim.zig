const KirbySim = @import("KirbySim.zig");
const KirbySimOpaque = opaque {};

pub const Host = extern struct {
    alloc: ?*const fn (context: ?*anyopaque, size: c_int) callconv(.C) ?*anyopaque,
    free: ?*const fn (context: ?*anyopaque, ptr: ?*anyopaque) callconv(.C) void,
    context: ?*anyopaque,
};

export fn kirby_sim_init(host: Host) ?*KirbySimOpaque {
    const sim = KirbySim.init(host) catch return null;
    return @as(?*KirbySimOpaque, @ptrCast(sim));
}

export fn kirby_sim_deinit(kirby_sim: ?*KirbySimOpaque) void {
    const sim = @as(?*KirbySim, @alignCast(@ptrCast(kirby_sim))) orelse return;
    sim.deinit();
}
