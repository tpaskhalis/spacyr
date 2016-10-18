#include <Rcpp.h>
#include <Python.h>
#include "redirect.hpp"
#include "converters.hpp"

#include <dlfcn.h>

// Based on:
// http://gallery.rcpp.org/articles/matplotlib-from-R/
// http://gallery.rcpp.org/articles/rcpp-python/

using namespace Rcpp;

//[[Rcpp::export]]
void initialize_python() {
  Py_Initialize();
  PyObject *m = PyImport_AddModule("__main__");
  PyObject *main = PyModule_GetDict(m);
  PyObject *f = PyCFunction_New(redirect_pystdout, (PyObject*)NULL);
  PyObject *f2 = PyCFunction_New(redirect_pystderr, (PyObject*)NULL);
  PyDict_SetItemString(main, "_Rcout",  f);
  PyDict_SetItemString(main, "_Rcerr",  f2);
  pyrun("class _StdoutCatcher:\n  def write(self, out):\n    _Rcout(out)");
  pyrun("class _StderrCatcher:\n  def write(self, out):\n    _Rcerr(out)");
  pyrun("import sys\nsys.stdout = _StdoutCatcher()");
  pyrun("import sys\nsys.stderr = _StderrCatcher()");
  
  //Workaround for the problem with lib-dynload/*.so, as documented here: 
  //https://mail.python.org/pipermail/new-bugs-announce/2008-November/003322.html
  dlopen("libpython2.7.so", RTLD_LAZY |RTLD_GLOBAL);
}

//[[Rcpp::export]]
void finalize_python() {
  Py_Finalize();
}

//[[Rcpp::export]]
void pyrun(std::string command) {
  PyRun_SimpleString(command.c_str());
}

//' Push data to python __main__ namespace
//' 
//' @param name Python variable name as string
//' @param x Numeric vector to copy to Python
//' 
//' @examples
//' numvec_to_python("l", 1:10)
//' pyrun("print l")
//' 
//[[Rcpp::export(name="topy.numeric")]]
void numvec_to_python(NumericVector x, std::string name){
  to_main(name, to_list(x));
}

//[[Rcpp::export(name="topy.character")]]
void charvec_to_python(std::vector<std::string> strings, std::string name){
  to_main(name, to_list(strings));
}

//Add NumericVector to dict in Python
//Used to "hide" variables for plotting
//[[Rcpp::export(name="pydict.numeric")]]
void num_to_dict(NumericVector x, std::string name, std::string dictname){
  add_to_dict(name, dictname, to_list(x));
}

//Add character vector to dict in Python
//Used to "hide" variables for plotting
//[[Rcpp::export(name="pydict.character")]]
void char_to_dict(std::vector<std::string>  x, std::string name, std::string dictname){
  add_to_dict(name, dictname, to_list(x));
}