from iarray import *

def test():
  arr = list()
  
  arr.append(0)
  arr.append(-1)
  arr.append(-3)
  arr.append(-2)
  arr.append(5)
  
  return arr, 5

def test_1():
  arr = list()
  
  arr.append(0)
  arr.append(-1)
  arr.append(3)
  arr.append(-2)
  arr.append(5)
  
  return arr, 5

def main():
  arr, n = test_1()

  print("Source 1")
  print_array(arr, n)
  
  new_arr, new_n = form_array(arr, n)

  print("Result")
  print_array(new_arr, new_n)

  arr, n = test()
 
  print("Source 2")
  print_array(arr, n)
  
  new_arr, new_n = form_array(arr, n)

  print("Result")
  print_array(new_arr, new_n)


if __name__ == '__main__':
  main()
