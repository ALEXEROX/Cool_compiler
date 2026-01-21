class Class inherits Object {
	param : Int;
	
	set_param(p : Int) : Int {
		param <- p;
	};
	
	get_param() : Int {
		param;
	};
	
};

class Main inherits IO {
	print(c : Class, i : Int, s : String) : IO {
		out_string("Class param = ");
		out_int(c.get_param());
		out_string("\n Int = ");
		out_int(i);
		out_string("\n String = ");
		out_string(s);
		out_string("\n\n");
	};

    modify(c : Class, i : Int, s : String) : IO {
		c.set_param(in_int());
		i <- in_int();
		in_string();
		s <- in_string();
		print(c, i, s);
	};

    main() : Int {
        {
			let c : Class <- new Class, i : Int <- 2, s : String <- "Hello" in {
				c.set_param(1);
				print(c, i, s);
				modify(c, i, s);
				print(c, i, s);
			};
			0;
        };
    };
};