const math = @import("math.zig");
const Input = @import("Input.zig");
const Self = @This();

pos: math.Vec2,

pub fn init(self: *Self) void {
    self.pos = math.Vec2{ 0, 0 };
}

pub fn update(self: *Self, input: Input) void {
    if (input.actionPressed(.MoveLeft)) {
        self.pos[0] -= 1;
    }
    if (input.actionPressed(.MoveRight)) {
        self.pos[0] += 1;
    }
    if (input.actionPressed(.MoveUp)) {
        self.pos[1] -= 1;
    }
    if (input.actionPressed(.MoveDown)) {
        self.pos[1] += 1;
    }
}
