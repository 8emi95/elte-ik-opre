int length(char* pointer)
{
	char* start;
	start = pointer; //megvan, hogy honnan indult
	int len = 0;
	while(*pointer++) //ha igaz, akkor 0, ha nem igaz akkor bármi más	
		//++len; //prefixes gyorsabb
	//return len;
	return pointer-start--; //az eltérésük bájtokban pont a karakterek száma	//a -- a *pointer++ miatt kell

}
