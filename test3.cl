class Main inherits IO {
    factorial(a : Int) : Int {
		let num : Int in {
		if (a < 2)
		then {
			num <- 1;
		}
		else {
			num <- a * factorial(a-1);
			}
		fi;
		num;
		};
	};

    main() : Int {
        {
			let n : Int in {
				n <- in_int();
				out_int(factorial(n));
			};
			0;
        };
    };
};