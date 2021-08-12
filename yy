[Wall] id = 0 | start.x = 482 | start.y = 441 | end.x = 492 | end.y = 196 | texture_id = 1
[Wall] id = 1 | start.x = 492 | start.y = 196 | end.x = 693 | end.y = 218 | texture_id = 1
[Wall] id = 2 | start.x = 693 | start.y = 218 | end.x = 680 | end.y = 389 | texture_id = 1
[Wall] id = 3 | start.x = 680 | start.y = 389 | end.x = 482 | end.y = 441 | texture_id = 1
[Wall] id = 4 | start.x = 693 | start.y = 218 | end.x = 840 | end.y = 190 | texture_id = 1
[Wall] id = 5 | start.x = 840 | start.y = 190 | end.x = 855 | end.y = 436 | texture_id = 1
[Wall] id = 6 | start.x = 855 | start.y = 436 | end.x = 680 | end.y = 389 | texture_id = 1
[Wall] id = 7 | start.x = 680 | start.y = 389 | end.x = 693 | end.y = 218 | texture_id = 1
[Wall] id = 8 | start.x = 840 | start.y = 190 | end.x = 975 | end.y = 271 | texture_id = 1
[Wall] id = 9 | start.x = 975 | start.y = 271 | end.x = 982 | end.y = 372 | texture_id = 1
[Wall] id = 10 | start.x = 982 | start.y = 372 | end.x = 855 | end.y = 436 | texture_id = 1
[Wall] id = 11 | start.x = 855 | start.y = 436 | end.x = 840 | end.y = 190 | texture_id = 1
[Wall] id = 12 | start.x = 975 | start.y = 271 | end.x = 1144 | end.y = 147 | texture_id = 1
[Wall] id = 13 | start.x = 1144 | start.y = 147 | end.x = 1132 | end.y = 456 | texture_id = 1
[Wall] id = 14 | start.x = 1132 | start.y = 456 | end.x = 982 | end.y = 372 | texture_id = 1
[Wall] id = 15 | start.x = 982 | start.y = 372 | end.x = 975 | end.y = 271 | texture_id = 1

[Room] id = 0 | first_wall_id = 0 | wall_count = 4 | floor_height = 100 | roof_height = 200 | has_ceiling = 1 | slope_floor = 1 | slope_roof = 1
[Room] id = 1 | first_wall_id = 4 | wall_count = 4 | floor_height = 100 | roof_height = 200 | has_ceiling = 1 | slope_floor = 1 | slope_roof = 1
[Room] id = 2 | first_wall_id = 8 | wall_count = 4 | floor_height = 100 | roof_height = 200 | has_ceiling = 1 | slope_floor = 1 | slope_roof = 1
[Room] id = 3 | first_wall_id = 12 | wall_count = 4 | floor_height = 100 | roof_height = 200 | has_ceiling = 1 | slope_floor = 1 | slope_roof = 1

[Portal] id = 0 | start.x = 680 | start.y = 389 | end.x = 693 | end.y = 218 | portal_type = 1
[Portal] id = 1 | start.x = 855 | start.y = 436 | end.x = 840 | end.y = 190 | portal_type = 1
[Portal] id = 2 | start.x = 982 | start.y = 372 | end.x = 975 | end.y = 271 | portal_type = 1

[Enemy] id = 0 | x = 1108 | y = 306 | rot = 13 | hp = 75 | wep.type_id = 1 | ai.type_id = 0 | sprite_id = 0

[Effect] id = 0 | type_id = 3 | loc.x = 487 | loc.y = 318 | target.x = 7 | target.y = 50 | target_id = 0
[Effect] id = 1 | type_id = 3 | loc.x = 766 | loc.y = 204 | target.x = 11 | target.y = 50 | target_id = 4
[Effect] id = 2 | type_id = 3 | loc.x = 907 | loc.y = 230 | target.x = 9 | target.y = 50 | target_id = 8
[Effect] id = 3 | type_id = 3 | loc.x = 1059 | loc.y = 209 | target.x = 6 | target.y = 50 | target_id = 12

[Pickup] id = 0 | loc.x = 723 | loc.y = 309 | flavor = 0 | weapon_type_id = 0
[Pickup] id = 1 | loc.x = 783 | loc.y = 369 | flavor = 1 | weapon_type_id = 1
[Pickup] id = 2 | loc.x = 820 | loc.y = 247 | flavor = 2 | weapon_type_id = 1
[Pickup] id = 3 | loc.x = 872 | loc.y = 324 | flavor = 3 | weapon_type_id = 0

[Player] spawn.x = 529 | spawn.y = 323 | rot = 360
