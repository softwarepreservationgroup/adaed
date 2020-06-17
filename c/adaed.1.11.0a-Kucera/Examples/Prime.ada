-- From d0micke@dtek.chalmers.se Fri Feb 28 11:50:45 1992


with  text_io;
use text_io;


procedure MICKE is
    package basic_num_io is new integer_io(integer);
    use   basic_num_io;
    IND: array (2..1000) of NATURAL;
    PTR: INTEGER :=2;
    ANT: INTEGER :=2;
    T: INTEGER :=0;
    TR: INTEGER:=4;
begin
    while ANT/=0 loop
        new_line;
        put("Give me the highest number to test (max 1000): ");
        get(ANT);
        while PTR<=ANT loop
            T:=PTR;
            while T<=ANT loop
                T:=T+PTR;
                if T<1000 then IND(T):=1;
                end if;
            end loop;
            while TR=4 loop
                PTR:=PTR+1;
                exit when PTR>=1000;
                exit when IND(PTR)=0;
            end loop;
        end loop;
        for I in 2..ANT loop
            if IND(I)=0 then put(I);
            new_line;
            end if;
            IND(I):=0;
        end loop;
        PTR:=2;
        T:=0;
    end loop;
end MICKE;
