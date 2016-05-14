ow_create_static_library(owserializer)

ow_use_public_libraries(
	owserialization
)

ow_use_private_libraries(
	tinyxml
)

ow_add_sources(
	src/DateXMLSerializer.cpp
	src/TimeXMLSerializer.cpp
)

ow_create_project_binary()
