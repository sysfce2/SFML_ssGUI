#!/bin/bash

set -e

./BackendTests/BackendFontTest_Auto
./BackendTests/BackendImageTest_Auto
./BackendTests/BackendMainWindowTest_Auto
./EventCallbackTests/BackgroundColorChangedEventTest_Auto
./EventCallbackTests/BaseEventCallbackTest_Auto
./EventCallbackTests/ChildAddedEventTest_Auto
./EventCallbackTests/ChildPositionChangedEventTest_Auto
./EventCallbackTests/ChildRemovedEventTest_Auto
./EventCallbackTests/MinMaxSizeChangedEventTest_Auto
./EventCallbackTests/OnFontChangeEventTest_Auto
./EventCallbackTests/OnObjectDestroyEventTest_Auto
./GUIObjectTests/TextTest_Auto
./GUIObjectTests/WidgetTest_Auto
./GUIObjectTests/WindowTest_Auto