project('keysniff', 'c',
  version : '0.1.0',
  default_options : [
    'warning_level=3',       # High level of warnings
    'c_std=c23',             # Modern C standard
    'buildtype=release',     # Optimizations enabled (-O3), debug info disabled
    'b_lto=true',            # Enable Link Time Optimization (smaller/faster binary)
    'strip=true'             # Strip symbols to reduce binary size
  ]
)

# CURL
curl_dep = dependency('libcurl', fallback : ['curl', 'curl_dep'])

# Define the include directory
inc_dir = include_directories('include')

# Define source files
sources = files(
  'src/main.c',
  'src/input.c',
  'src/network.c',
  'src/utils.c'
)

# Create the executable
executable('keysniff',
  sources,
  include_directories : inc_dir,
  dependencies : curl_dep,
  install : false
)