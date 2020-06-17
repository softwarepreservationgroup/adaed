generic
  type OBJECT is limited private;
  type NAME   is access OBJECT;
procedure UNCHECKED_DEALLOCATION(X : in out NAME);
generic
  type SOURCE is limited private;
  type TARGET is limited private;
function UNCHECKED_CONVERSION(S : SOURCE) return TARGET;

procedure UNCHECKED_DEALLOCATION(X : in out NAME) is
begin
  if x /= null then x := null; end if;
end UNCHECKED_DEALLOCATION;

function UNCHECKED_CONVERSION(S : SOURCE) return TARGET is
  T : TARGET;
begin
  raise PROGRAM_ERROR;
  return T;
end UNCHECKED_CONVERSION;

