const math = @import("math.zig");
const Input = @import("Input.zig");
const Self = @This();

pos: math.Vec2,

pub fn init(self: *Self) void {
    self.pos = math.Vec2{ 0, 0 };
}

pub fn update(self: *Self, input: Input) void {
    if (input.actionPressed(.move_left)) {
        self.pos[0] -= 1;
    }
    if (input.actionPressed(.move_right)) {
        self.pos[0] += 1;
    }
    if (input.actionPressed(.move_up)) {
        self.pos[1] -= 1;
    }
    if (input.actionPressed(.move_down)) {
        self.pos[1] += 1;
    }
}
