enum COLOR {
	PINK = 0,
	YELLOW,
	ORANGE,
	GREEN,
	BLUE,
	PURPLE,
	RED
};

struct charm {
    enum COLOR color;
    char shape[32];
};

struct next_charm_args {
	struct charm charm_arg[7];
	int index_arg;
};

program DISPLAY_PRG {
        version DISPLAY_VER {
          struct charm next_charm( next_charm_args ) = 1;
		  void read_charm( struct charm ) = 2;
        } = 1;
} = 0x20000101;