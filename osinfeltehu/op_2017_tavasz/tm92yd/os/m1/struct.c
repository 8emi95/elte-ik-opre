struct tipusnev {
    mezotipus1 mezonev1;
    ...
    mezotipusN mezonevN;
}

struct tipusnev azonosito;

azonosito.mezonev;

--

struct tipusnev *ptr;

(*ptr).mezonev; == ptr-> mezonev;