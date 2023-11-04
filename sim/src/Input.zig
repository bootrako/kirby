const kirby_sim = @import("kirby_sim.zig");
const std = @import("std");
const Self = @This();

pub const Action = kirby_sim.InputAction;
const input_action_len = @typeInfo(Action).Enum.fields.len;

host: kirby_sim.Host,
input_buffer: [input_action_len * 2]bool,
cur_input: []bool,
prv_input: []bool,

pub fn init(self: *Self, host: kirby_sim.Host) void {
    self.host = host;
    for (self.input_buffer[0..]) |*value| {
        value.* = false;
    }
    self.prv_input = self.input_buffer[0..input_action_len];
    self.cur_input = self.input_buffer[input_action_len..];
}

pub fn update(self: *Self) void {
    std.mem.swap([*]bool, &self.cur_input.ptr, &self.prv_input.ptr);
    for (self.cur_input, 0..) |*value, i| {
        value.* = self.host.input_action_pressed.?(self.host.context, @enumFromInt(i)) != 0;
    }
}

pub fn actionPressed(self: Self, action: Action) bool {
    const index: usize = @intCast(@intFromEnum(action));
    return self.cur_input[index];
}

pub fn actionJustPressed(self: Self, action: Action) bool {
    const index: usize = @intCast(@intFromEnum(action));
    return self.cur_input[index] and !self.prv_input[index];
}

pub fn actionJustReleased(self: Self, action: Action) bool {
    const index: usize = @intCast(@intFromEnum(action));
    return !self.cur_input[index] and self.prv_input[index];
}
