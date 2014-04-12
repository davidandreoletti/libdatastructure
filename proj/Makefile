#
#  Makefile
#  Builds library for various platforms
#
#  For full documentation: make usage

#
THIS_MAKEFILE_DIR = $(dir $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

INTERNAL_PREFIX_DIR_NAME=prefix
INTERNAL_PREFIX_DIR=$(THIS_MAKEFILE_DIR)../$(INTERNAL_PREFIX_DIR_NAME)

PREFIX_DIR_NAME=prefix
PREFIX=$(THIS_MAKEFILE_DIR)../$(PREFIX_DIR_NAME)

# Usage
usage : 
	@echo "iOS platform:"
	@echo " make build-ios [PREFIX=/mypath/to/prefix/directory]"
	@echo "  PREFIX : Absolute path to directory to copy built library to. Default: $(PREFIX)"

#
# iOS 
#

# Clean iOS build
clean-ios :
	rm -rf $(INTERNAL_PREFIX_DIR)


# Build for iOS
build-ios : clean-ios 
	cp -rv ./../include/ $(INTERNAL_PREFIX_DIR)/
	xcrun --verbose xcodebuild -verbose -project ./proj.ios/datastructureapi-core-lib/datastructureapi-core-lib.xcodeproj -target datastructureapi -configuration Release clean build || false
	([ "$(INTERNAL_PREFIX_DIR)" != "$(PREFIX)" ] && cp -v $(INTERNAL_PREFIX_DIR)/* $(PREFIX)/ ) || [ "$(INTERNAL_PREFIX_DIR)" == "$(PREFIX)" ]
