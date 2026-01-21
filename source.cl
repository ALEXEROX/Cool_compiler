class Animal inherits IO {
    name : String;
    age : Int;

    init(n : String, a : Int) : SELF_TYPE {
        {
            name <- n;
            age <- a;
			age <- in_int();
            self;
        };
    };

    describe() : IO {
        {
            out_string("Animal: ");
            out_string(name);
            out_string(", age = ");
            out_int(age);
            out_string("\n");
        };
    };
	
	speak() : String { out_string("Hello! "); 
	"o";};
	
	get_age() : Int {age;};
};

class Lion inherits Animal {
    speak() : String { "Roaaar!"; };
	
    hunt() : String { "The lion hunts its prey."; };
};

class Elephant inherits Animal {
    speak() : String { "Pawooo!"; };

    remember() : String { "The elephant never forgets."; };
};

class Parrot inherits Animal {
    phrase : String;
	
	init(n : String, a : Int) : SELF_TYPE {
	self@Animal.init(n,a);
	phrase <- "Hello!";
	self;
	};

    set_phrase(p : String) : Parrot {
        {
            phrase <- p;
            self;
        };
    };

    speak() : String { 
	self@Animal.speak();
	phrase; 
	};
};

-- Simple linked list implementation
class List inherits Object {
    head_elem : Animal;
    tail_list : List;
    empty : Bool;
	
	init() : SELF_TYPE {
	empty <- true;
	self;
	};

    is_empty() : Bool { empty; };

    head() : Animal {
        if empty then { abort(); new Animal; }
        else head_elem fi;
    };
	
	set_head(a : Animal) : Animal {head_elem <- a;}; 

    tail() : List {
        if empty then { abort(); new List; }
        else tail_list fi;
    };

    cons(a : Animal) : List {
        (new List).init_cons(a, self);
    };

    init_cons(a : Animal, t : List) : List {
        {
            head_elem <- a;
            tail_list <- t;
            empty <- false;
            self;
        };
    };
};

-- The Zoo class manages a list of animals
class Zoo inherits IO {
    animals : List;
	
	init() : SELF_TYPE {
	animals <- new List;
	animals.init();
	self;
	};

    add(a : Animal) : Zoo {
        {
            animals <- animals.cons(a);
            self;
        };
    };

    show_all() : Zoo {
        let current : List <- animals in
        while not current.is_empty() loop {
            let a : Animal <- current.head() in
            {
                a.describe();
                out_string("It says: ");
                out_string(a.speak());
                out_string("\n\n");
            };
            current <- current.tail();
        } pool;
		self;
    };
	
	get_zoo() : List {animals;};
};

-- Main program entry point
class Main inherits IO {
    zoo : Zoo;

    main() : Int {
        {
			zoo <- (new Zoo).init();
            zoo.add((new Lion).init("Simba", 5));
            zoo.add((new Elephant).init("Dumbo", 10));
            zoo.add((new Parrot).init("Kesha", 2).set_phrase("Polly wants a cracker!"));

            out_string("Welcome to the Cool Zoo!\n\n");
            zoo.show_all();
				
			let arr : StringArray <- new StringArray, elem : String in{
				elem <- arr.append("Hello");
				out_string(arr.get(0));
			};
			
			0;
        };
    };
};