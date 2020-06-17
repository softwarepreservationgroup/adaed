with TEXT_IO;
with SYSTEM;

procedure TEST is
	package NAME_IO is new text_io.enumeration_io(SYSTEM.NAME);
begin
        TEXT_IO.PUT("Hello World");
        TEXT_IO.NEW_LINE;
        TEXT_IO.PUT("System is ");
        NAME_IO.PUT(SYSTEM.SYSTEM_NAME);
        TEXT_IO.NEW_LINE;
end;
