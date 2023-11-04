const kirby_sim = @import("kirby_sim.zig");
const Input = @import("Input.zig");
const Level = @import("Level.zig");
const Player = @import("Player.zig");
const Self = @This();

pub const time_per_frame = 1.0 / 60.0;

host: kirby_sim.Host,
frame_accumulator: f32,
input: Input,
level: Level,
player: Player,

fn validateHost(host: kirby_sim.Host) !void {
    inline for (@typeInfo(kirby_sim.Host).Struct.fields) |field| {
        if (@field(host, field.name) == null) {
            return error.InvalidHost;
        }
    }
}

pub fn init(host: kirby_sim.Host) !*Self {
    try validateHost(host);

    const ptr = host.alloc.?(host.context, @sizeOf(Self)) orelse return error.OutOfMemory;
    const self = @as(*Self, @alignCast(@ptrCast(ptr)));

    self.host = host;
    self.frame_accumulator = 0;
    self.input.init(host);
    self.level.init(host, .GreenGreens);
    self.player.init();

    return self;
}

pub fn deinit(self: *Self) void {
    self.host.free.?(self.host.context, self);
}

pub fn update(self: *Self, delta_time: f32) !i32 {
    var num_frames: i32 = 0;

    self.frame_accumulator += delta_time;
    while (self.frame_accumulator > time_per_frame) {
        try self.updateFrame();
        num_frames += 1;
        self.frame_accumulator -= time_per_frame;
    }

    return num_frames;
}

fn updateFrame(self: *Self) !void {
    self.input.update();
    self.level.update();
    self.player.update(self.input);
}
