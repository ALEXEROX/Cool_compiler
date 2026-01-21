class Main inherits IO {
    is_true() : Bool {
		out_string("I do \n");
		true;
	};

    main() : Int {
        {
			let a : Int, b : Int in {
				a <- in_int();
				b <- in_int();
				if a < b && is_true()
				then out_string("Yes")
				else out_string("I not do")
				fi;
			};
			0;
        };
    };
};