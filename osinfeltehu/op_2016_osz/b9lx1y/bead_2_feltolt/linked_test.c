#include "visitor.h"
#include "visitor_list.h"
#include "event.h"
#include "event_list.h"

#include <stdio.h>
#include <time.h>

int main() {
  node_t *events = create_event_list_empty();

  event_t event1 = create_event(2, "Wasabi latogatas");
  event_t event2 = create_event(4, "Mopsz bemutato");
  event_t event3 = create_event(5, "Relaxacio");

  append_event(&events, &event1);
  append_event(&events, &event2);
  append_event(&events, &event3);
  
  write_events_to_file(&events, "events.bin");

  free_events(&events);

  read_events_from_file(&events, "events.bin");

  print_events(&events);

  node_t *visitors = create_visitor_list_empty();

  visitor_t visitor1 = create_visitor(2, "Endre", "gamesh411@gmail.com");
  visitor_t visitor2 = create_visitor(4, "Zoli", "english-2@live.com");
  visitor_t visitor3 = create_visitor(2, "Niki", "killerfosgalacsin@gmail.com");
  visitor_t visitor4 = create_visitor(4, "Brigi", "brigi@brigi.brigi");
  visitor_t visitor5 = create_visitor(5, "Kati", "vkati@provimax.hu");

  append_visitor(&visitors, &visitor1);
  append_visitor(&visitors, &visitor2);
  append_visitor(&visitors, &visitor3);
  append_visitor(&visitors, &visitor4);
  append_visitor(&visitors, &visitor5);

  delete_visitor_all(&visitors, 4);

  write_visitors_to_file(&visitors, "visitors.bin");

  free_visitors(&visitors);

  read_visitors_from_file(&visitors, "visitors.bin");

  print_visitors(&visitors);

  free_events(&events);
  free_visitors(&visitors);

  return 0;
}
