local config = require "core.config"

config.plugins.build.targets = {
  { name = "Re-Craft-3DS_3dsx", build_directory = "build", type = "cmake" },
  { name = "send", build_directory = "build", type = "cmake" }
}
config.plugins.debugger.remote_ip = "192.168.178.60:4003"

