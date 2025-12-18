cmake_minimum_required(VERSION 3.5)

project(InventoryManagement VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Copy necessary files
configure_file(data.csv ${CMAKE_CURRENT_BINARY_DIR}/data.csv COPYONLY)
configure_file(dataBase.db ${CMAKE_CURRENT_BINARY_DIR}/dataBase.db COPYONLY)

# Project sources
set(PROJECT_SOURCES
        main.cpp
        product.h product.cpp
        user.h user.cpp
        seller.h seller.cpp
        products.h products.cpp
        resource.qrc
        datahandler.h datahandler.cpp
        manufacturers.h manufacturers.cpp
        warehouse_graph.h
        warehouse_graph.cpp

        # GUI
        GUI/home_window.h GUI/home_window.cpp GUI/home_window.ui
        GUI/inventory_widget.h GUI/inventory_widget.cpp GUI/inventory_widget.ui
        GUI/add_product_window.h GUI/add_product_window.cpp GUI/add_product_window.ui
        GUI/edit_product_window.h GUI/edit_product_window.cpp GUI/edit_product_window.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(InventoryManagement
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(InventoryManagement
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(InventoryManagement PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)
