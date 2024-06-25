#!/bin/bash
ctest   --build-and-test tests build/tests \
	--build-generator "Unix Makefiles" \
	--test-command ctest \
	--output-on-failure
