set(CMAKE_FOLDER "examples")

function(example TESTNAME SOURCE EXENAME)
  add_executable(${EXENAME} ${SOURCE})
  add_test(NAME cartesian_product_view.examples.${TESTNAME} COMMAND ${EXENAME})
endfunction(example)

example(prob_dist_sum_dice prob_dist_sum_dice.cpp prob_dist_sum_dice)