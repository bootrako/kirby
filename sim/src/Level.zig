const kirby_sim = @import("kirby_sim.zig");
const math = @import("math.zig");
const Self = @This();

const tile_size = math.Vec2{ 16, 16 };

collision_map: []bool,

pub fn init(self: *Self, host: kirby_sim.Host, data_file: kirby_sim.DataFile) void {
    _ = self;
    const data = host.open_data_file.?(host.context, data_file);
    host.log.?(host.context, data);
}

pub fn update(self: *Self) void {
    _ = self;
}
