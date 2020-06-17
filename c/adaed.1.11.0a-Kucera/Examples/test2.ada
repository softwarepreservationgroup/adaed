--This file causes a binding error which we haven't figured out yet.
--The same error has popped up in other programs and seems to deal
--with the handling of generics.

generic
        with function one_value(x,y : integer) return float;
package gene is

        procedure test(x,y : integer);

end gene;

with text_io;
package body gene is

        package flt_io is new text_io.float_io(float);

        procedure test(x,y : integer) is
                  u : float;
        begin
                text_io.put("un essai :");
--                u := one_value(x,y);
                flt_io.put(u);
                text_io.new_line;
        end test;

end gene;

with gene;
procedure error is
          function my_value(x,y : integer) return float is
          begin
                return float(x+y);
          end my_value;

          package my_pkg is new gene(one_value => my_value);
begin
        my_pkg.test(1,5);
end error;
