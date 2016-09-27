#' @useDynLib spacyr
#' @importFrom Rcpp sourceCpp
NULL

.onLoad <- function(libname, pkgname) {
  tryCatch((initialize_python()),
           error = function(e) "Error: Python has not been found.")
}