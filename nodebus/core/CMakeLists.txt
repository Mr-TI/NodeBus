#
# Copyright (C) 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#

# ### FILES ###
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
file(GLOB project_HDRS *.h)
file(GLOB project_SRCS *.cpp)

# ### QT4 ###
find_package(Qt4 COMPONENTS QtCore REQUIRED)
include(${QT_USE_FILE})
qt4_wrap_ui(project_UIS_H)
qt4_wrap_cpp(project_MOC_SRCS application.h slaveapplication.h)

add_definitions(-DUSE_NODEBUS_EXCEPTION)

add_subdirectory(jsonparser)
add_subdirectory(idlparser)

# ### TARGET ###
add_library(nodebus ${NODEBUS_LIBRARY_TYPE} ${project_SRCS} ${project_HDRS} ${project_MOC_SRCS})
target_link_libraries(nodebus ${QT_LIBRARIES} jsonparser idlparser)

install(TARGETS nodebus 
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
