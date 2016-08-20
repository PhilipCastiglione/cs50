#!/usr/bin/env ruby

CIPHERS = [
    "andi:HALRCq0IBXEPM",
    "caesar:50zPJlUFIYY0o",
    "eli:50MxVjGD7EfY6",
    "hdan:50z2Htq2DN2qs",
    "jason:50CMVwEqJXRUY",
    "john:50TGdEyijNDNY",
    "levatich:50QykIulIPuKI",
    "rob:50q.zrL5e0Sak",
    "skroob:50Bpa7n/23iug",
    "zamyla:HAYRs6vZAb4wo"
].freeze

CIPHERS.each do |c|
    t = Time.now.to_i
    puts `./crack #{c.split(":").last}`
    puts "cracked #{c} in #{Time.now.to_i - t} seconds"
end