#define inita 0
#define initb 120

int act_a = inita;
int act_b = initb;
int niters = 10;

for (int i = 0; i < niters; i++)
{
  subint = (act_b-act_a) / 120;
  my_left = act_a + i * subint;
  my_right = my_left + subint

  if (f(my_left) < 0 && f(my_right) > 0){
    act_a = my_left;
    act_b = my_right;
  }
}
return act_a;



