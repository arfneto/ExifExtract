#include "stdafx.h"
#include <iostream>
#include "TagMeans.h"


TagMeans::TagMeans()
{
	InteropIndex = "???";

	dict.insert(
		std::pair<uint16_t,FieldDict>(	0x100,
			{ 
			"A",
			"Image width",
			"ImageWidth",
			"256",
			"100",
			"SHORT or LONG",
			"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x101,
			{
				"A",
				"Image height",
				"ImageLength",
				"257",
				"101",
			"SHORT or LONG",
			"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x102,
			{
				"A",
				"Number of bits per component",
				"BitsPerSample",
				"258",
				"102",
				"SHORT",
				"3"
			})
		);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x103,
			{
				"A",
				"Compression Scheme",
				"Compression",
				"259",
				"103",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x106,
			{
				"A",
				"Pixel Composition",
				"PhotometricInterpretation",
				"262",
				"106",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x112,
			{
				"A",
				"Orientation of Image",
				"Orientation",
				"274",
				"112",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x115,
			{
				"A",
				"Number of Components",
				"SamplesPerPixel",
				"277",
				"115",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x11C,
			{
				"A",
				"Image data arrangement",
				"PlanarConfiguration",
				"284",
				"11C",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x212,
			{
				"A",
				"Subsampling ratio of Y to C",
				"YCbCrSubSampling",
				"530",
				"212",
				"SHORT",
				"2"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x213,
			{
				"A",
				"Y and C positioning",
				"YCbCrPositioning",
				"531",
				"213",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x11A,
			{
				"A",
				"Image resolution in width direction",
				"XResolution",
				"282",
				"11A",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x11B,
			{
				"A",
				"Image resolution in height direction",
				"YResolution",
				"283",
				"11B",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x128,
			{
				"A",
				"Unit of X and Y resolution",
				"ResolutionUnit",
				"296",
				"128",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x111,
			{
				"B",
				"Image data location",
				"StripOffsets",
				"273",
				"111",
				"SHORT or LONG",
				"S"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x116,
			{
				"B",
				"Number of rows per strip",
				"RowsPerStrip",
				"278",
				"116",
				"SHORT or LONG",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x117,
			{
				"B",
				"Bytes per compressed strip",
				"StripByteCounts",
				"279",
				"117",
				"SHORT or LONG",
				"S"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x201,
			{
				"B",
				"Offset to JPEG SOI",
				"JPEGInterchangeFormat",
				"513",
				"201",
				"LONG",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x202,
			{
				"B",
				"Bytes of JPEG data",
				"JPEGInterchangeFormatLength",
				"514",
				"202",
				"LONG",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x12D,
			{
				"C",
				"Transfer Function",
				"TransferFunction",
				"301",
				"12D",
				"SHORT",
				"3 * 256"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x13E,
			{
				"C",
				"White point chromaticity",
				"TransferFunction",
				"318",
				"13E",
				"RATIONAL",
				"2"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x13F,
			{
				"C",
				"Chromaticity of primaries",
				"PrimaryChromaticities",
				"319",
				"13F",
				"RATIONAL",
				"6"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x211,
			{
				"C",
				"Color space transformation matrix coefficients",
				"YCbCrCoefficients",
				"529",
				"211",
				"RATIONAL",
				"3"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x214,
			{
				"C",
				"Pair of black and white reference values",
				"ReferenceBlackWhite",
				"532",
				"214",
				"RATIONAL",
				"2"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x132,
			{
				"D",
				"File change date and time",
				"DateTime",
				"306",
				"132",
				"ASCII",
				"20"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x10E,
			{
				"D",
				"Image title",
				"ImageDescription",
				"270",
				"10E",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x10F,
			{
				"D",
				"Image input equipment manufacturer",
				"Make",
				"271",
				"10F",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x110,
			{
				"D",
				"Image input equipment model",
				"Model",
				"272",
				"110",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x131,
			{
				"D",
				"Software used",
				"Software",
				"305",
				"131",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x13B,
			{
				"D",
				"Person who created the image",
				"Artist",
				"315",
				"13B",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x8298,
			{
				"D",
				"Copyright holder",
				"Copyright",
				"33432",
				"8298",
				"ASCII",
				"Any"
			})
	);

	// Table 59, Exif IFD Pointer
	dict.insert(
		std::pair<uint16_t, FieldDict>(0x8769,
			{
				"59",
				"Exif IFD Pointer",
				"ExifInfo",
				"34665",
				"8769",
				"LONG",
				"1"
			})
	);

	// Table 59, GPS Info IFD Pointer
	dict.insert(
		std::pair<uint16_t, FieldDict>(0x8825,
			{
				"59",
				"GPS Info IFD Pointer",
				"GPSInfo",
				"34853",
				"8825",
				"LONG",
				"1"
			})
	);

	//
	// Table 7 Exif IFD Attributes
	//

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9000,
			{
				"EA",
				"Exif version",
				"ExifVersion",
				"36864",
				"9000",
				"UNDEFINED",
				"4"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA000,
			{
				"EA",
				"Supported Flashpix version",
				"FlashpixVersion",
				"40960",
				"A000",
				"UNDEFINED",
				"4"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA001,
			{
				"EB",
				"Color space information",
				"ColorSpace",
				"40961",
				"A001",
				"SHORT",
				"1"
			})
	);

	// Table 59, Interoperability IFD Pointer
	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA005,
			{
				"59",
				"Interoperability IFD Pointer",
				"InteropIFD",
				"40965",
				"A005",
				"LONG",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA500,
			{
				"EB",
				"Gamma",
				"Gamma",
				"42240",
				"A500",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9101,
			{
				"EC",
				"Meaning of each component",
				"ComponentsConfiguration",
				"37121",
				"9101",
				"UNDEFINED",
				"4"
			})
	);


	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9102,
			{
				"EC",
				"Image compression mode",
				"CompressedBitsPerPixel",
				"37122",
				"9102",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA002,
			{
				"EC",
				"Valid image width",
				"PixelXDimension",
				"40962",
				"A002",
				"SHORT or LONG",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA003,
			{
				"EC",
				"Valid image height",
				"PixelYDimension",
				"40963",
				"A003",
				"SHORT or LONG",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x927C,
			{
				"ED",
				"Manufacturer notes",
				"MakerNote",
				"37500",
				"927C",
				"UNDEFINED",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9286,
			{
				"ED",
				"User comments",
				"UserCommment",
				"37510",
				"9286",
				"UNDEFINED",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA004,
			{
				"EE",
				"Related audio file",
				"RelatedSoundFile",
				"40964",
				"A004",
				"ASCII",
				"13"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9003,
			{
				"EF",
				"Date and time of original data generation",
				"DateTimeOriginal",
				"36867",
				"9003",
				"ASCII",
				"20"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9004,
			{
				"EF",
				"Date and time of digital data generation",
				"DateTimeDigitized",
				"36868",
				"9004",
				"ASCII",
				"20"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9010,
			{
				"EF",
				"Offset data of DateTime",
				"OffsetTime",
				"36880",
				"9010",
				"ASCII",
				"7"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9011,
			{
				"EF",
				"Offset data of DateTimeOriginal",
				"OffsetTimeOriginal",
				"36881",
				"9011",
				"ASCII",
				"7"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9012,
			{
				"EF",
				"Offset data of DateTimeDigitized",
				"OffsetTimeDigitized",
				"36882",
				"9012",
				"ASCII",
				"7"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9290,
			{
				"EF",
				"DateTime subseconds",
				"SubSecTime",
				"37520",
				"9290",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9291,
			{
				"EF",
				"DateTimeOriginal subseconds",
				"SubSecTimeOriginal",
				"37521",
				"9291",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9292,
			{
				"EF",
				"DateTimeDigitized subseconds",
				"SubSecTimeDigitized",
				"37522",
				"9292",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9400,
			{
				"EG2",
				"Temperature",
				"Temperature",
				"37888",
				"9400",
				"SRATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9401,
			{
				"EG2",
				"Humidity",
				"Humidity",
				"37889",
				"9401",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9402,
			{
				"EG2",
				"Pressure",
				"Pressure",
				"37890",
				"9402",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9403,
			{
				"EG2",
				"WaterDepth",
				"WaterDepth",
				"37891",
				"9403",
				"SRATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9404,
			{
				"EG2",
				"Acceleration",
				"Acceleration",
				"37892",
				"9404",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9405,
			{
				"EG2",
				"Camera elevation angle",
				"CameraElevationAngle",
				"37893",
				"9405",
				"SRATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA420,
			{
				"EH",
				"Unique image ID",
				"ImageUniqueID",
				"42016",
				"A420",
				"ASCII",
				"33"
			})
	);


	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA430,
			{
				"EH",
				"Camera Owner Name",
				"CameraOwnerName",
				"42032",
				"A430",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA431,
			{
				"EH",
				"Body Serial Number",
				"BodySerialNumber",
				"42033",
				"A431",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA432,
			{
				"EH",
				"Lens Specification",
				"LensSpecification",
				"42034",
				"A432",
				"RATIONAL",
				"4"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA433,
			{
				"EH",
				"Lens Make",
				"LensMake",
				"42035",
				"A433",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA434,
			{
				"EH",
				"Lens Model",
				"LensModel",
				"42036",
				"A434",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA435,
			{
				"EH",
				"Lens Serial Number",
				"LensSerialNumber",
				"42037",
				"A435",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x829A,
			{
				"EG",
				"Exposure Time",
				"ExposureTime",
				"33434",
				"829A",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x829D,
			{
				"EG",
				"F number",
				"FNumber",
				"33437",
				"829D",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x8822,
			{
				"EG",
				"Exposure Program",
				"ExposureProgram",
				"34850",
				"8822",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x8824,
			{
				"EG",
				"Spectral Sensitivity",
				"SpectralSensitivity",
				"34852",
				"8824",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x8827,
			{
				"EG",
				"Photographic Sensitivity",
				"PhotographicSensitivity",
				"34855",
				"8827",
				"SHORT",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x8828,
			{
				"EG",
				"Optoeletric conversion factor",
				"OECF",
				"34856",
				"8828",
				"UNDEFINED",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x8830,
			{
				"EG",
				"Sensitivity Type",
				"SensitivityType",
				"34864",
				"8830",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x8831,
			{
				"EG",
				"Standard Output Sensitivity",
				"StandardOutputSensitivity",
				"34865",
				"8831",
				"LONG",
				"1"
			})
	);


	dict.insert(
		std::pair<uint16_t, FieldDict>(0x8832,
			{
				"EG",
				"Recommended Exposure Index",
				"Recommended Exposure Index",
				"34866",
				"8832",
				"LONG",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x8833,
			{
				"EG",
				"ISO Speed",
				"ISOSpeed",
				"34867",
				"8833",
				"LONG",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x8834,
			{
				"EG",
				"ISO Speed Latitude yyy",
				"ISOSpeedLatitudeyyy",
				"34868",
				"8834",
				"LONG",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x8835,
			{
				"EG",
				"ISO Speed Latitude zzz",
				"ISOSpeedLatitudezzz",
				"34869",
				"8835",
				"LONG",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9201,
			{
				"EG",
				"Shutter Speed",
				"ShutterSpeedValue",
				"37377",
				"9201",
				"SRATIONAL",
				"1"
			})
	); 

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9202,
			{
				"EG",
				"Aperture",
				"ApertureValue",
				"37378",
				"9202",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9203,
			{
				"EG",
				"Brightness",
				"BrightnessValue",
				"37379",
				"9203",
				"SRATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9204,
			{
				"EG",
				"Exposure Bias",
				"ExposureBiasValue",
				"37380",
				"9204",
				"SRATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9205,
			{
				"EG",
				"Maximum lens aperture",
				"MaxApertureValue",
				"37381",
				"9205",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9206,
			{
				"EG",
				"Subject distance",
				"SubjectDistance",
				"37382",
				"9206",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9207,
			{
				"EG",
				"Metering Mode",
				"MeteringMode",
				"37383",
				"9207",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9208,
			{
				"EG",
				"Light source",
				"LightSource",
				"37384",
				"9208",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9209,
			{
				"EG",
				"Flash",
				"Flash",
				"37385",
				"9209",
				"SHORT",
				"1"
			})
	);


	dict.insert(
		std::pair<uint16_t, FieldDict>(0x920A,
			{
				"EG",
				"Lens focal length",
				"FocalLength",
				"37386",
				"920A",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x9214,
			{
				"EG",
				"Subject area",
				"SubjectArea",
				"37396",
				"9214",
				"SHORT",
				"2, 3, 4"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA20B,
			{
				"EG",
				"Flash Energy",
				"FlashEnergy",
				"41483",
				"A20B",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA20C,
			{
				"EG",
				"Spatial frequency response",
				"SpatialFrequencyResponse",
				"41484",
				"A20C",
				"UNDEFINED",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA20E,
			{
				"EG",
				"Focal plane X resolution",
				"FocalPlaneXResolution",
				"41486",
				"A20E",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA20F,
			{
				"EG",
				"Focal plane Y resolution",
				"FocalPlaneYResolution",
				"41487",
				"A20F",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA210,
			{
				"EG",
				"Focal plane resolution unit",
				"FocalPlaneResolutionUnit",
				"41488",
				"A210",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA214,
			{
				"EG",
				"Subject location",
				"SubjectLocation",
				"41492",
				"A214",
				"SHORT",
				"2"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA215,
			{
				"EG",
				"Exposure index",
				"ExposureIndex",
				"41493",
				"A215",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA217,
			{
				"EG",
				"Sensing method",
				"SensingMethod",
				"41495",
				"A217",
				"SHORT",
				"1"
			})
	);


	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA300,
			{
				"EG",
				"File source",
				"FileSource",
				"41728",
				"A300",
				"UNDEFINED",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA301,
			{
				"EG",
				"Scene Type",
				"SceneType",
				"41729",
				"A301",
				"UNDEFINED",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA302,
			{
				"EG",
				"CFA pattern",
				"CFAPattern",
				"41730",
				"A302",
				"UNDEFINED",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA401,
			{
				"EG",
				"Custom image processing",
				"CustomRendered",
				"41985",
				"A401",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA402,
			{
				"EG",
				"Exposure Mode",
				"ExposureMode",
				"41986",
				"A402",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA403,
			{
				"EG",
				"White balance",
				"WhiteBalance",
				"41987",
				"A403",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA404,
			{
				"EG",
				"Digital zoom ratio",
				"DigitalZoomRatio",
				"41988",
				"A404",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA405,
			{
				"EG",
				"Focal Length in 35mm film",
				"FocalLength35mmFilm",
				"41989",
				"A405",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA406,
			{
				"EG",
				"Scene capture type",
				"SceneCaptureType",
				"41990",
				"A406",
				"SHORT",
				"1"
			})
	);


	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA407,
			{
				"EG",
				"Gain control",
				"GainControl",
				"41991",
				"A407",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA408,
			{
				"EG",
				"Contrast",
				"Contrast",
				"41992",
				"A408",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA409,
			{
				"EG",
				"Saturation",
				"Saturation",
				"41993",
				"A409",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA40A,
			{
				"EG",
				"Sharpness",
				"Sharpness",
				"41994",
				"A40A",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA40B,
			{
				"EG",
				"Device settings Description",
				"DeviceSettingDescription",
				"41995",
				"A40B",
				"UNDEFINED",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0xA40C,
			{
				"EG",
				"Subject distance range",
				"SubjectDistanceRange",
				"41996",
				"A40C",
				"SHORT",
				"1"
			})
	);

	//
	// Table 15 GPS attribute Information
	//

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0000,
			{
				"GA",
				"GPS tag version",
				"GPSVersionID",
				"0",
				"0",
				"BYTE",
				"4"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0001,
			{
				"GA",
				"North or South Latitude",
				"GPSLatitudeRef",
				"1",
				"1",
				"ASCII",
				"2"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0002,
			{
				"GA",
				"Latitude",
				"Latitude",
				"2",
				"2",
				"RATIONAL",
				"3"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0003,
			{
				"GA",
				"East or West Longitude",
				"GPSLongitudeRef",
				"3",
				"3",
				"ASCII",
				"2"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0004,
			{
				"GA",
				"Longitude",
				"Longitude",
				"4",
				"4",
				"RATIONAL",
				"3"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0005,
			{
				"GA",
				"Altitude Reference",
				"GPSAltitudeRef",
				"5",
				"5",
				"BYTE",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0006,
			{
				"GA",
				"Altitude",
				"GPSAltitude",
				"6",
				"6",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0007,
			{
				"GA",
				"GPS time (atomic clock)",
				"GPSTimeStamp",
				"7",
				"7",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0008,
			{
				"GA",
				"GPS satellites used for measurement",
				"GPSSatellites",
				"8",
				"8",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0009,
			{
				"GA",
				"GPS receiver status",
				"GPSStatus",
				"9",
				"9",
				"ASCII",
				"2"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x000A,
			{
				"GA",
				"GPS measurement mode",
				"GPSMeasureMode",
				"10",
				"A",
				"ASCII",
				"2"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x000B,
			{
				"GA",
				"Measurement precision",
				"GPSDOP",
				"11",
				"B",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x000C,
			{
				"GA",
				"Speed unit",
				"GPSSpeedRef",
				"12",
				"C",
				"ASCII",
				"2"
			})
	);


	dict.insert(
		std::pair<uint16_t, FieldDict>(0x000D,
			{
				"GA",
				"Speed of GPS receiver",
				"GPSSpeed",
				"13",
				"D",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x000E,
			{
				"GA",
				"Reference for direction of movement",
				"GPSTrackRef",
				"14",
				"E",
				"ASCII",
				"2"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x000F,
			{
				"GA",
				"Direction of movement",
				"GPSTrack",
				"15",
				"F",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0010,
			{
				"GA",
				"Reference for direction of image",
				"GPSImgDirectionRef",
				"16",
				"10",
				"ASCII",
				"2"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0011,
			{
				"GA",
				"Direction of image",
				"GPSImgDirection",
				"17",
				"11",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0012,
			{
				"GA",
				"Geodetic survey data used",
				"GPSMapDatum",
				"18",
				"12",
				"ASCII",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0013,
			{
				"GA",
				"Reference for latitude of destination",
				"GPSDestLatitudeRef",
				"19",
				"13",
				"ASCII",
				"2"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0014,
			{
				"GA",
				"Latitude of destination",
				"GPSDestLatitude",
				"20",
				"14",
				"RATIONAL",
				"3"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0015,
			{
				"GA",
				"Reference for Longitude of destination",
				"GPSDestLongitudeRef",
				"21",
				"15",
				"ASCII",
				"2"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0016,
			{
				"GA",
				"Longitude of destination",
				"GPSDestLongitude",
				"22",
				"16",
				"RATIONAL",
				"3"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0017,
			{
				"GA",
				"Reference for bearing of destination",
				"GPSDestBearingRef",
				"23",
				"17",
				"ASCII",
				"2"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0018,
			{
				"GA",
				"Bearing of destination",
				"GPSDestBearing",
				"24",
				"18",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x0019,
			{
				"GA",
				"Reference for distance to destination",
				"GPSDestDistanceRef",
				"25",
				"19",
				"ASCII",
				"2"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x001A,
			{
				"GA",
				"Distance to destination",
				"GPSDestDistance",
				"26",
				"1A",
				"RATIONAL",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x001B,
			{
				"GA",
				"Name of GPS processing method",
				"GPSProccessingMethod",
				"27",
				"1B",
				"UNDEFINED",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x001C,
			{
				"GA",
				"Name of GPS area",
				"GPSAreaInformation",
				"28",
				"1C",
				"UNDEFINED",
				"Any"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x001D,
			{
				"GA",
				"GPS date",
				"GPSDateStamp",
				"29",
				"1D",
				"ASCII",
				"11"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x001E,
			{
				"GA",
				"GPS differential correction",
				"GPSDifferential",
				"30",
				"1E",
				"SHORT",
				"1"
			})
	);

	dict.insert(
		std::pair<uint16_t, FieldDict>(0x001F,
			{
				"GA",
				"Horizontal positioning error",
				"GPSHPositioningError",
				"31",
				"1F",
				"RATIONAL",
				"1"
			})
	);

	//
	// Table 16 Interoperability IFD
	//
	// should go to another dictionary since the codes overlap
	//


 }

TagMeans::~TagMeans()
{
}

void TagMeans::print()
{
	cout << 
		"Total of " << dict.size() <<
		" entries in map" <<
		endl;

	for (auto it : dict)
	{
		printTag(it.second);
	}
}

void TagMeans::printTag(FieldDict & tag)
{
	printf(
		"%3s. %50s %30s %5s %4s.H %20s %8s\n",
		tag._category.c_str(),
		tag._tagName.c_str(),
		tag._fieldName.c_str(),
		tag._decID.c_str(),
		tag._hexID.c_str(),
		tag._type.c_str(),
		tag._count.c_str()
	);
	return;
}

bool TagMeans::explainTag(uint16_t & n, FieldDict & v)
{
	auto it = dict.find(n);
	if (it == dict.end())
	{
		return false;
	}
	v = it->second;
	return true;
}

void TagMeans::setInterop(char val[4])
{
	// value was set to  "???" in the constructor
	// possible values are R98 R03 and THM
	if (val[0] == 'R')
	{
		if (val[1] == '0')
		{
			if (val[2] == '3')
			{
				InteropIndex = "R03";
			}
		}
		else
		{
			if (val[1] == '9')
			{
				if (val[2] == '8')
				{
					InteropIndex = "R98";
				}
			}
		}
	}
	else
	{
		if (val[0] == 'T')
		{
			if (val[1] == 'H')
			{
				if (val[2] == 'M')
				{
					InteropIndex = "THM";
				}
			}
		}
	}
	return;
}
