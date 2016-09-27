#include <Rcpp.h>
#include <Python.h>

#include <dlfcn.h>

// Based on:
// http://gallery.rcpp.org/articles/matplotlib-from-R/
// http://gallery.rcpp.org/articles/rcpp-python/

using namespace Rcpp;

//[[Rcpp::export]]
void initialize_python() {
  Py_Initialize();
  
  //Workaround for the problem with lib-dynload/*.so, as documented here: 
  //https://mail.python.org/pipermail/new-bugs-announce/2008-November/003322.html
  dlopen("libpython2.7.so", RTLD_LAZY |RTLD_GLOBAL);
}

//[[Rcpp::export]]
void finalize_python() {
  Py_Finalize();
}

//[[Rcpp::export]]
void pycall(std::string command) {
  PyRun_SimpleString(command.c_str());
}
