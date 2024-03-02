#! /bin/bash
test -e ${1} && (g++ ${1} -o main 2> /dev/null || echo Compile Error) || echo No cpp
test -d ./result || mkdir result
echo

if test -f main; then
  i=0
  while [ ${i} != 5 ]  # Set max i to your total data number
  do
    if timeout 2s ./main < ./open_case/case${i}.txt > ./result/res${i}.txt; then
      if diff -b -B ./result/res${i}.txt ./open_case/golden${i}.txt; then
        echo -e "\033[34m Pass case${i}"
      else 
        echo Failed case${i} \(WA\)
      fi
    else
      echo Failed case${i} \(TLE or RE\)
    fi 
    i=$(($i+1))    
  done
  echo
fi
