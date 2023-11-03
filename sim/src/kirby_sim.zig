const KirbySim = @import("KirbySim.zig");
const KirbySimOpaque = opaque {};

pub const InputAction = enum(c_int) {
    move_left,
    move_right,
    move_up,
    move_down,
};

pub const Host = extern struct {
    alloc: ?*const fn (context: ?*anyopaque, size: c_int) callconv(.C) ?*anyopaque,
    free: ?*const fn (context: ?*anyopaque, ptr: ?*anyopaque) callconv(.C) void,
    panic: ?*const fn (context: ?*anyopaque, msg: [*:0]const u8) callconv(.C) void,
    input_action_pressed: ?*const fn (context: ?*anyopaque, input_action: InputAction) callconv(.C) bool,
    context: ?*anyopaque,
};

pub const Pos = extern struct {
    x: c_int,
    y: c_int,
};

fn asKirbySim(kirby_sim_opaque: ?*const KirbySimOpaque) *const KirbySim {
    return @as(?*const KirbySim, @alignCast(@ptrCast(kirby_sim_opaque))) orelse @panic("null sim ptr!");
}

fn asKirbySimMut(kirby_sim_opaque: ?*KirbySimOpaque) *KirbySim {
    return @as(?*KirbySim, @alignCast(@ptrCast(kirby_sim_opaque))) orelse @panic("null sim ptr!");
}

fn handleErr(kirby_sim: *const KirbySim, e: anyerror) noreturn {
    kirby_sim.host.panic.?(kirby_sim.host.context, @errorName(e));
    unreachable;
}

export fn kirby_sim_init(host: Host) ?*KirbySimOpaque {
    const sim = KirbySim.init(host) catch return null;
    return @as(?*KirbySimOpaque, @ptrCast(sim));
}

export fn kirby_sim_deinit(kirby_sim_opaque: ?*KirbySimOpaque) void {
    const kirby_sim = asKirbySimMut(kirby_sim_opaque);
    kirby_sim.deinit();
}

export fn kirby_sim_update(kirby_sim_opaque: ?*KirbySimOpaque, delta_time: f32) c_int {
    const kirby_sim = asKirbySimMut(kirby_sim_opaque);
    return kirby_sim.update(delta_time) catch |e| handleErr(kirby_sim, e);
}

export fn kirby_sim_get_player_pos(kirby_sim_opaque: ?*const KirbySimOpaque) Pos {
    const kirby_sim = asKirbySim(kirby_sim_opaque);
    return Pos{ .x = kirby_sim.player_pos[0], .y = kirby_sim.player_pos[1] };
}
