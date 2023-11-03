const kirby_sim = @import("kirby_sim.zig");
const std = @import("std");
const Self = @This();

pub const InputAction = kirby_sim.InputAction;
const input_action_len = @typeInfo(InputAction).Enum.fields.len;

pub const time_per_frame = 1.0 / 60.0;

host: kirby_sim.Host,
frame_accumulator: f32,
input_buffer: [input_action_len * 2]bool,
cur_input: []bool,
prv_input: []bool,
player_pos: @Vector(2, i32),

fn validateHost(host: kirby_sim.Host) !void {
    if (host.alloc == null or host.free == null or host.panic == null or host.input_action_pressed == null) {
        return error.InvalidHost;
    }
}

fn pollInput(self: *Self) void {
    std.mem.swap([*]bool, &self.cur_input.ptr, &self.prv_input.ptr);
    for (self.cur_input, 0..) |*value, i| {
        value.* = self.host.input_action_pressed.?(self.host.context, @enumFromInt(i));
    }
}

pub fn inputActionPressed(self: *const Self, input_action: InputAction) bool {
    const index: usize = @intCast(@intFromEnum(input_action));
    return self.cur_input[index];
}

pub fn inputActionJustPressed(self: *const Self, input_action: InputAction) bool {
    const index: usize = @intCast(@intFromEnum(input_action));
    return self.cur_input[index] and !self.prv_input[index];
}

pub fn inputActionJustReleased(self: *const Self, input_action: InputAction) bool {
    const index: usize = @intCast(@intFromEnum(input_action));
    return !self.cur_input[index] and self.prv_input[index];
}

pub fn init(host: kirby_sim.Host) !*Self {
    try validateHost(host);

    const ptr = host.alloc.?(host.context, @sizeOf(Self)) orelse return error.OutOfMemory;
    const self = @as(*Self, @alignCast(@ptrCast(ptr)));

    self.host = host;
    self.frame_accumulator = 0;
    for (self.input_buffer[0..]) |*value| {
        value.* = false;
    }
    self.prv_input = self.input_buffer[0..input_action_len];
    self.cur_input = self.input_buffer[input_action_len..];
    self.player_pos = @Vector(2, i32){ 0, 0 };

    return self;
}

pub fn deinit(self: *Self) void {
    self.host.free.?(self.host.context, self);
}

pub fn update(self: *Self, delta_time: f32) !i32 {
    var num_frames: i32 = 0;

    self.frame_accumulator += delta_time;
    while (self.frame_accumulator > time_per_frame) {
        self.pollInput();
        try self.frame();
        num_frames += 1;
        self.frame_accumulator -= time_per_frame;
    }

    return num_frames;
}

pub fn frame(self: *Self) !void {
    if (self.inputActionPressed(.move_left)) {
        self.player_pos[0] -= 1;
    }
    if (self.inputActionPressed(.move_right)) {
        self.player_pos[0] += 1;
    }
    if (self.inputActionPressed(.move_up)) {
        self.player_pos[1] -= 1;
    }
    if (self.inputActionPressed(.move_down)) {
        self.player_pos[1] += 1;
    }
}
