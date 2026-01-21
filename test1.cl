-- Main program entry point
class Main inherits IO {

    main() : Int {
        {			
			let my_arr : IntArray <- new IntArray, i : Int <- 0, j : Int <- 0, tmp : Int in {
				while i < 10 loop {
					out_string("Введите элемент массива: ");
					my_arr.append(in_int());
					i <- i + 1;
				}
				pool;
				
				i <- 0;
				
				while i < my_arr.length() - 1 loop {
					while j < my_arr.length()- i - 1 loop {
						if (my_arr.get(j) < my_arr.get(j+1))
						then
						{
							tmp <- my_arr.get(j);
							my_arr.set(j,my_arr.get(j+1));
							my_arr.set(j+1,tmp);
						}
						else
						{
							0;
						}
						fi;
						j <- j + 1;
				}
				pool;
				i <- i + 1;
				j <- 0;
				}
				pool;
				i<-0;
				while i < my_arr.length() loop {
					out_int(my_arr.get(i));
					out_string(" ");
					i <- i + 1;
				}
				pool;
			};
			
			0;
        };
    };
};