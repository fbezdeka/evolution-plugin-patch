#
# Evolution patch module
#
# Copyright (c) Florian Bezdeka, 2025
#
# SPDX-License-Identifier: LGPL-3.0-only
#

add_library(${PROJECT_NAME} MODULE
	xpatch-module.c
	xpatch-parser.c
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	G_LOG_DOMAIN=\"${PROJECT_NAME}\"
)

target_compile_options(${PROJECT_NAME} PUBLIC
	${EVOLUTION_SHELL_CFLAGS}
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${EVOLUTION_SHELL_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
	${EVOLUTION_SHELL_LDFLAGS}
)

install(TARGETS ${PROJECT_NAME}
	DESTINATION ${EVOLUTION_MODULE_DIR}
)
