generic
    type Elem is private;
    type Index is (<>);
    type Vector is array (Index range <>) of Elem;

function HasRepetition (T: in vector) return boolean;