procedure transport is

type item (Max:Positive; name: String := "Empty") is record
	current: Natural:=1;
end record;

type location is (storage_point, storage_detector, accumulator_detector, accumulator_point);

empty:item(1, "Empty");
cobblestone:item(64, "Cobblestone");
potato:item(64,"Potato");
cooked_potato:item(64,"Cooked Potato");
oak_log:item(64,"Oak Log");
coal:item(64,"Coal");

task accumulator;
	entry get_item(to:in out item; l: out boolean);
task mass_storage;
task type mass_storage_access_point;
task type hopper_transport
	entry sendto(dest:Positive);
	entry timeout;
end hopper_transport;

task body hopper_transport is
	current: location := storage_point;
begin
	accept sendto
end hopper_transport;

left, right: Hopper transport;


begin
	null;
end;