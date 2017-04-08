#include <retrodyn/game.h>
#include <retrodyn/component.h>

#include <stdio.h>

declare_body(test, {
    int value;
});

declare_update(test, {
    printf("update called: value = %d\n", self->value);
});

declare_tick(test, {
    printf("tick called: value = %d\n", self->value);
});

declare_head(test);

declare_body(test2, {
});

declare_update(test2, { });

declare_tick(test2, { });

declare_head(test2);

void start() {
    test *mytest = c_new(test);

    mytest->value = 15;

    c_update(mytest);
    c_tick(mytest);

    printf("mytest is a test:  %d\n", c_isa(mytest, test));
    printf("mytest is a test2: %d\n", c_isa(mytest, test2));

    c_free(mytest);
}
