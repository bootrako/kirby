const kirby_sim = @import("kirby_sim.zig");
const Self = @This();

host: kirby_sim.Host,

pub fn init(host: kirby_sim.Host) !*Self {
    try validateHost(host);

    const ptr = host.alloc.?(host.context, @sizeOf(Self)) orelse return error.OutOfMemory;
    const self = @as(*Self, @alignCast(@ptrCast(ptr)));

    self.host = host;

    return self;
}

pub fn deinit(self: *Self) void {
    self.host.free.?(self.host.context, self);
}

fn validateHost(host: kirby_sim.Host) !void {
    if (host.alloc == null or host.free == null) {
        return error.InvalidHost;
    }
}
