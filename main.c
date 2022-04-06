#include "opengl.h"
#include "main.h"
#include "cfd.h"
#include <io.h>

//#define INPUT_USE_DIRECTINPUTKEYBOARD

HPBUFFERARB hpbuffer = NULL;

HINSTANCE hInstDDraw = NULL;
HINSTANCE hInstDInput = NULL;

LPDIRECTINPUT8 lpDirectInput = NULL;
LPDIRECTINPUTDEVICE8 lpDIKeyboard = NULL;
LPDIRECTDRAW7 lpDirectDraw = NULL;

GUID G_DirectInput8		= {0xBF798030, 0x483A, 0x4DA2, 0xAA, 0x99, 0x5D, 0x64, 0xED, 0x36, 0x97, 0x00};
GUID G_Key				= {0x55728220, 0xD33C, 0x11CF, 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00};
GUID G_SysKeyboard		= {0x6F1D2B61, 0xD5A0, 0x11CF, 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00};

HRESULT (WINAPI *gDirectDrawCreateEx)(GUID *lpGuid, LPVOID *lplpDD, REFIID iid, IUnknown *pUnkOuter);
HRESULT (WINAPI *gDirectInput8Create)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter);

static DIOBJECTDATAFORMAT keyboard_odf[] = {
	{&GUID_Key, 0,   0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(0),		0},
	{&GUID_Key, 1,   0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(1),		0},
	{&GUID_Key, 2,   0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(2),		0},
	{&GUID_Key, 3,   0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(3),		0},
	{&GUID_Key, 4,   0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(4),		0},
	{&GUID_Key, 5,   0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(5),		0},
	{&GUID_Key, 6,   0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(6),		0},
	{&GUID_Key, 7,   0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(7),		0},
	{&GUID_Key, 8,   0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(8),		0},
	{&GUID_Key, 9,   0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(9),		0},
	{&GUID_Key, 10,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(10),	0},
	{&GUID_Key, 11,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(11),	0},
	{&GUID_Key, 12,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(12),	0},
	{&GUID_Key, 13,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(13),	0},
	{&GUID_Key, 14,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(14),	0},
	{&GUID_Key, 15,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(15),	0},
	{&GUID_Key, 16,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(16),	0},
	{&GUID_Key, 17,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(17),	0},
	{&GUID_Key, 18,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(18),	0},
	{&GUID_Key, 19,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(19),	0},
	{&GUID_Key, 20,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(20),	0},
	{&GUID_Key, 21,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(21),	0},
	{&GUID_Key, 22,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(22),	0},
	{&GUID_Key, 23,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(23),	0},
	{&GUID_Key, 24,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(24),	0},
	{&GUID_Key, 25,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(25),	0},
	{&GUID_Key, 26,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(26),	0},
	{&GUID_Key, 27,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(27),	0},
	{&GUID_Key, 28,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(28),	0},
	{&GUID_Key, 29,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(29),	0},
	{&GUID_Key, 30,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(30),	0},
	{&GUID_Key, 31,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(31),	0},
	{&GUID_Key, 32,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(32),	0},
	{&GUID_Key, 33,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(33),	0},
	{&GUID_Key, 34,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(34),	0},
	{&GUID_Key, 35,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(35),	0},
	{&GUID_Key, 36,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(36),	0},
	{&GUID_Key, 37,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(37),	0},
	{&GUID_Key, 38,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(38),	0},
	{&GUID_Key, 39,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(39),	0},
	{&GUID_Key, 40,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(40),	0},
	{&GUID_Key, 41,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(41),	0},
	{&GUID_Key, 42,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(42),	0},
	{&GUID_Key, 43,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(43),	0},
	{&GUID_Key, 44,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(44),	0},
	{&GUID_Key, 45,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(45),	0},
	{&GUID_Key, 46,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(46),	0},
	{&GUID_Key, 47,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(47),	0},
	{&GUID_Key, 48,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(48),	0},
	{&GUID_Key, 49,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(49),	0},
	{&GUID_Key, 50,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(50),	0},
	{&GUID_Key, 51,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(51),	0},
	{&GUID_Key, 52,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(52),	0},
	{&GUID_Key, 53,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(53),	0},
	{&GUID_Key, 54,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(54),	0},
	{&GUID_Key, 55,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(55),	0},
	{&GUID_Key, 56,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(56),	0},
	{&GUID_Key, 57,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(57),	0},
	{&GUID_Key, 58,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(58),	0},
	{&GUID_Key, 59,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(59),	0},
	{&GUID_Key, 60,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(60),	0},
	{&GUID_Key, 61,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(61),	0},
	{&GUID_Key, 62,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(62),	0},
	{&GUID_Key, 63,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(63),	0},
	{&GUID_Key, 64,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(64),	0},
	{&GUID_Key, 65,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(65),	0},
	{&GUID_Key, 66,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(66),	0},
	{&GUID_Key, 67,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(67),	0},
	{&GUID_Key, 68,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(68),	0},
	{&GUID_Key, 69,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(69),	0},
	{&GUID_Key, 70,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(70),	0},
	{&GUID_Key, 71,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(71),	0},
	{&GUID_Key, 72,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(72),	0},
	{&GUID_Key, 73,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(73),	0},
	{&GUID_Key, 74,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(74),	0},
	{&GUID_Key, 75,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(75),	0},
	{&GUID_Key, 76,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(76),	0},
	{&GUID_Key, 77,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(77),	0},
	{&GUID_Key, 78,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(78),	0},
	{&GUID_Key, 79,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(79),	0},
	{&GUID_Key, 80,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(80),	0},
	{&GUID_Key, 81,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(81),	0},
	{&GUID_Key, 82,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(82),	0},
	{&GUID_Key, 83,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(83),	0},
	{&GUID_Key, 84,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(84),	0},
	{&GUID_Key, 85,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(85),	0},
	{&GUID_Key, 86,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(86),	0},
	{&GUID_Key, 87,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(87),	0},
	{&GUID_Key, 88,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(88),	0},
	{&GUID_Key, 89,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(89),	0},
	{&GUID_Key, 90,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(90),	0},
	{&GUID_Key, 91,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(91),	0},
	{&GUID_Key, 92,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(92),	0},
	{&GUID_Key, 93,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(93),	0},
	{&GUID_Key, 94,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(94),	0},
	{&GUID_Key, 95,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(95),	0},
	{&GUID_Key, 96,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(96),	0},
	{&GUID_Key, 97,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(97),	0},
	{&GUID_Key, 98,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(98),	0},
	{&GUID_Key, 99,  0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(99),	0},
	{&GUID_Key, 100, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(100),	0},
	{&GUID_Key, 101, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(101),	0},
	{&GUID_Key, 102, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(102),	0},
	{&GUID_Key, 103, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(103),	0},
	{&GUID_Key, 104, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(104),	0},
	{&GUID_Key, 105, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(105),	0},
	{&GUID_Key, 106, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(106),	0},
	{&GUID_Key, 107, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(107),	0},
	{&GUID_Key, 108, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(108),	0},
	{&GUID_Key, 109, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(109),	0},
	{&GUID_Key, 110, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(110),	0},
	{&GUID_Key, 111, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(111),	0},
	{&GUID_Key, 112, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(112),	0},
	{&GUID_Key, 113, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(113),	0},
	{&GUID_Key, 114, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(114),	0},
	{&GUID_Key, 115, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(115),	0},
	{&GUID_Key, 116, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(116),	0},
	{&GUID_Key, 117, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(117),	0},
	{&GUID_Key, 118, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(118),	0},
	{&GUID_Key, 119, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(119),	0},
	{&GUID_Key, 120, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(120),	0},
	{&GUID_Key, 121, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(121),	0},
	{&GUID_Key, 122, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(122),	0},
	{&GUID_Key, 123, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(123),	0},
	{&GUID_Key, 124, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(124),	0},
	{&GUID_Key, 125, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(125),	0},
	{&GUID_Key, 126, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(126),	0},
	{&GUID_Key, 127, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(127),	0},
	{&GUID_Key, 128, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(128),	0},
	{&GUID_Key, 129, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(129),	0},
	{&GUID_Key, 130, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(130),	0},
	{&GUID_Key, 131, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(131),	0},
	{&GUID_Key, 132, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(132),	0},
	{&GUID_Key, 133, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(133),	0},
	{&GUID_Key, 134, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(134),	0},
	{&GUID_Key, 135, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(135),	0},
	{&GUID_Key, 136, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(136),	0},
	{&GUID_Key, 137, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(137),	0},
	{&GUID_Key, 138, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(138),	0},
	{&GUID_Key, 139, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(139),	0},
	{&GUID_Key, 140, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(140),	0},
	{&GUID_Key, 141, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(141),	0},
	{&GUID_Key, 142, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(142),	0},
	{&GUID_Key, 143, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(143),	0},
	{&GUID_Key, 144, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(144),	0},
	{&GUID_Key, 145, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(145),	0},
	{&GUID_Key, 146, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(146),	0},
	{&GUID_Key, 147, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(147),	0},
	{&GUID_Key, 148, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(148),	0},
	{&GUID_Key, 149, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(149),	0},
	{&GUID_Key, 150, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(150),	0},
	{&GUID_Key, 151, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(151),	0},
	{&GUID_Key, 152, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(152),	0},
	{&GUID_Key, 153, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(153),	0},
	{&GUID_Key, 154, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(154),	0},
	{&GUID_Key, 155, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(155),	0},
	{&GUID_Key, 156, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(156),	0},
	{&GUID_Key, 157, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(157),	0},
	{&GUID_Key, 158, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(158),	0},
	{&GUID_Key, 159, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(159),	0},
	{&GUID_Key, 160, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(160),	0},
	{&GUID_Key, 161, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(161),	0},
	{&GUID_Key, 162, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(162),	0},
	{&GUID_Key, 163, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(163),	0},
	{&GUID_Key, 164, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(164),	0},
	{&GUID_Key, 165, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(165),	0},
	{&GUID_Key, 166, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(166),	0},
	{&GUID_Key, 167, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(167),	0},
	{&GUID_Key, 168, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(168),	0},
	{&GUID_Key, 169, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(169),	0},
	{&GUID_Key, 170, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(170),	0},
	{&GUID_Key, 171, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(171),	0},
	{&GUID_Key, 172, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(172),	0},
	{&GUID_Key, 173, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(173),	0},
	{&GUID_Key, 174, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(174),	0},
	{&GUID_Key, 175, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(175),	0},
	{&GUID_Key, 176, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(176),	0},
	{&GUID_Key, 177, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(177),	0},
	{&GUID_Key, 178, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(178),	0},
	{&GUID_Key, 179, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(179),	0},
	{&GUID_Key, 180, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(180),	0},
	{&GUID_Key, 181, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(181),	0},
	{&GUID_Key, 182, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(182),	0},
	{&GUID_Key, 183, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(183),	0},
	{&GUID_Key, 184, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(184),	0},
	{&GUID_Key, 185, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(185),	0},
	{&GUID_Key, 186, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(186),	0},
	{&GUID_Key, 187, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(187),	0},
	{&GUID_Key, 188, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(188),	0},
	{&GUID_Key, 189, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(189),	0},
	{&GUID_Key, 190, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(190),	0},
	{&GUID_Key, 191, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(191),	0},
	{&GUID_Key, 192, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(192),	0},
	{&GUID_Key, 193, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(193),	0},
	{&GUID_Key, 194, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(194),	0},
	{&GUID_Key, 195, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(195),	0},
	{&GUID_Key, 196, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(196),	0},
	{&GUID_Key, 197, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(197),	0},
	{&GUID_Key, 198, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(198),	0},
	{&GUID_Key, 199, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(199),	0},
	{&GUID_Key, 200, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(200),	0},
	{&GUID_Key, 201, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(201),	0},
	{&GUID_Key, 202, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(202),	0},
	{&GUID_Key, 203, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(203),	0},
	{&GUID_Key, 204, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(204),	0},
	{&GUID_Key, 205, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(205),	0},
	{&GUID_Key, 206, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(206),	0},
	{&GUID_Key, 207, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(207),	0},
	{&GUID_Key, 208, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(208),	0},
	{&GUID_Key, 209, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(209),	0},
	{&GUID_Key, 210, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(210),	0},
	{&GUID_Key, 211, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(211),	0},
	{&GUID_Key, 212, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(212),	0},
	{&GUID_Key, 213, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(213),	0},
	{&GUID_Key, 214, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(214),	0},
	{&GUID_Key, 215, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(215),	0},
	{&GUID_Key, 216, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(216),	0},
	{&GUID_Key, 217, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(217),	0},
	{&GUID_Key, 218, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(218),	0},
	{&GUID_Key, 219, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(219),	0},
	{&GUID_Key, 220, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(220),	0},
	{&GUID_Key, 221, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(221),	0},
	{&GUID_Key, 222, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(222),	0},
	{&GUID_Key, 223, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(223),	0},
	{&GUID_Key, 224, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(224),	0},
	{&GUID_Key, 225, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(225),	0},
	{&GUID_Key, 226, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(226),	0},
	{&GUID_Key, 227, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(227),	0},
	{&GUID_Key, 228, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(228),	0},
	{&GUID_Key, 229, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(229),	0},
	{&GUID_Key, 230, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(230),	0},
	{&GUID_Key, 231, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(231),	0},
	{&GUID_Key, 232, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(232),	0},
	{&GUID_Key, 233, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(233),	0},
	{&GUID_Key, 234, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(234),	0},
	{&GUID_Key, 235, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(235),	0},
	{&GUID_Key, 236, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(236),	0},
	{&GUID_Key, 237, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(237),	0},
	{&GUID_Key, 238, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(238),	0},
	{&GUID_Key, 239, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(239),	0},
	{&GUID_Key, 240, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(240),	0},
	{&GUID_Key, 241, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(241),	0},
	{&GUID_Key, 242, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(242),	0},
	{&GUID_Key, 243, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(243),	0},
	{&GUID_Key, 244, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(244),	0},
	{&GUID_Key, 245, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(245),	0},
	{&GUID_Key, 246, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(246),	0},
	{&GUID_Key, 247, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(247),	0},
	{&GUID_Key, 248, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(248),	0},
	{&GUID_Key, 249, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(249),	0},
	{&GUID_Key, 250, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(250),	0},
	{&GUID_Key, 251, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(251),	0},
	{&GUID_Key, 252, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(252),	0},
	{&GUID_Key, 253, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(253),	0},
	{&GUID_Key, 254, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(254),	0},
	{&GUID_Key, 255, 0x80000000 | DIDFT_BUTTON | DIDFT_MAKEINSTANCE(255),	0}
};

#define NUM_KOBJECTS (sizeof(keyboard_odf) / sizeof(keyboard_odf[0]))

const DIDATAFORMAT keyboard_df = {
	sizeof(DIDATAFORMAT),		//data format
	sizeof(DIOBJECTDATAFORMAT),	//object data format
	DIDF_RELAXIS,				//absolute axis coordinates
	256,						//device data size
	NUM_KOBJECTS,				//number of objects
	keyboard_odf,				//device object data format
};

_mem_alloc_t *mem_aligned = NULL;
setup_info_t si;
void *logmutex = NULL;

//byte order functions
short (*LittleShort)(const short var);
int (*LittleInt)(const int var);
float (*LittleFloat)(const float var);
short (*BigShort)(const short var);
int (*BigInt)(const int var);
float (*BigFloat)(const float var);
short SwapShort(const short var);
short PassShort(const short var);
int SwapInt(const int var);
int PassInt(const int var);
float SwapFloat(const float var);
float PassFloat(const float var);

extern int ThreadMain(void *reserved);

/*
----------------
_malloc_aligned

- allocate a chunk of 'x' memory aligned on 'a' byte boundary
- NOTE: does not zero the allocated memory

- x:			number of bytes to allocate
- a:			byte boundary to align on
- return:		pointer to the aligned memory chunk
----------------
*/
void *_malloc_aligned(const dword x, const dword a)
{
	_mem_alloc_t *ptr, *inc;

	//store info about each allocated chunk in a linked list for later freeing
	if(!(ptr = (_mem_alloc_t *)malloc(sizeof(_mem_alloc_t))))
	{
		Sys_Log("_malloc_aligned: Failed to allocate memory for the list element");
		return NULL;
	}

	ptr->next = NULL;

	if(!mem_aligned)
		mem_aligned = ptr;
	else
	{
		inc = mem_aligned;
		while(inc->next)
			inc = inc->next;

		inc->next = ptr;
	}

	if(!(ptr->orig = malloc(x + (a - 1))))
	{
		Sys_Log("_malloc_aligned: Failed to allocate memory for the aligned chunk");
		return NULL;
	}

	ptr->aligned = (void *)(((_long)ptr->orig + (a - 1)) & ~(a - 1));
	return ptr->aligned;
}

/*
----------------
_free_aligned

- free a chunk of aligned memory allocated with _malloc_aligned

- ptr:			pointer to the memory to be freed
----------------
*/
void _free_aligned(void *ptr)
{
	_mem_alloc_t *inc, *prev;

	//find the allocated chunk in the list
	inc = mem_aligned;
	prev = NULL;
	while(inc)
	{
		if(inc->aligned == ptr)
			break;

		prev = inc;
		inc = inc->next;
	}

	if(!inc)
		return;

	free(inc->orig);
	if(!prev)
		mem_aligned = inc->next;
	else
		prev->next = inc->next;

	//free the list element
	free(inc);
}

/*
----------------
MC2UC

- converts a multi-char string to wide-char

- string:		multi-char string
- return:		wide-char string
----------------
*/
LPCWSTR MC2UC(const char *string)
{
	int len;
	wchar_t *ucstring;

	len = (int)strlen(string) + 1;
	if(!(ucstring = (wchar_t *)calloc(len, sizeof(wchar_t))))
		return NULL;

	//multi-byte string to wide-character string
	mbstowcs(ucstring, string, len);
	return (LPCWSTR)ucstring;
}

/*
----------------
Sys_Error

- calls Sys_Shutdown, which ends the process

- error:		error string
----------------
*/
void __cdecl Sys_Error(const char *error, ...)
{
	char string[1024];
	va_list argptr;

	va_start(argptr, error);
	vsnprintf(string, sizeof(string), error, argptr);
	va_end(argptr);

	Sys_Log("\nERROR: %s", string);
	MessageBox(NULL, string, "Error", MB_ICONSTOP | MB_OK);

	Sys_Shutdown();
}

/*
----------------
Sys_Message

- creates a popup window with the given message
- also logs the message

- msg:			message string to display
----------------
*/
void __cdecl Sys_Message(const char *msg, ...)
{
	char string[1024];
	va_list argptr;

	va_start(argptr, msg);
	vsnprintf(string, sizeof(string), msg, argptr);
	va_end(argptr);

	MessageBox(NULL, string, "Message", MB_ICONINFORMATION | MB_OK);
	Sys_Log("MSG: %s", string);
}

/*
----------------
Sys_Log

- write a string to the log file
- NOTE: the file is opened and closed each call to flush the
		file stream to the HDD in case of an abnormal exit
- logmutex is used to prevent jumbled logging

- log:			log string
----------------
*/
void __cdecl Sys_Log(const char *log, ...)
{
	char string[1024];
	va_list argptr;
	FILE *fp;

	va_start(argptr, log);
	vsnprintf(string, sizeof(string), log, argptr);
	va_end(argptr);

//	Sys_LockMutex(logmutex);
	if(!(fp = fopen("log.txt", "a")))
	{
//		Sys_UnlockMutex(logmutex);
		return;
	}

	fprintf(fp, "%s\n", string);
	fclose(fp);
//	Sys_UnlockMutex(logmutex);
}

/*
----------------
Sys_Timestamp

- NOTE: allocates memory for the timestamp string

- return:		timestamp string
----------------
*/
char *Sys_Timestamp(void)
{
	SYSTEMTIME systime;
	char *ts;

	if(!(ts = (char *)calloc(64, sizeof(char))))
		Sys_Error("Sys_Timestamp: Failed to allocate system timestamp output string");

	GetLocalTime(&systime);
	sprintf(ts, "Timestamp: %d/%d/%d  %d:%d:%d.%d", systime.wMonth, systime.wDay, systime.wYear, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
	return ts;
}

/*
----------------
Sys_GetTime

- return:		time in milliseconds since start of the process
----------------
*/
int Sys_GetTime(void)
{
	static unsigned int base;
	static bool init = false;

	if(!init)
	{
		base = timeGetTime();
		init = true;
	}

	return timeGetTime() - base;
}

/*
----------------
Sys_Shutdown

- shuts everything down, then exits the process
----------------
*/
void Sys_Shutdown(void)
{
	SYSTEMTIME systime;

	Sys_Log("\nShutting down");

	Shutdown();

	Sys_Log("Shutting down input");
	In_Shutdown();

	Sys_Log("Shutting down video");
	Sys_VideoShutdown();

	Sys_Log("Shutdown complete");

	GetLocalTime(&systime);
	Sys_Log("Terminated: %d/%d/%d  %d:%d:%d.%d", systime.wMonth, systime.wDay, systime.wYear, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
	Sys_Log("--------------------------------------\n");

	exit(0);
}

/*
----------------
Sys_GetFilePathExtension

- path:			file path and name
- extension:	pre-allocated memory to return the extension in
- len:			number of bytes available in the extension buffer
- return:		true = extension found, false = no extension returned
----------------
*/
bool Sys_GetFilePathExtension(const char *path, char *extension, int len)
{
	int length;
	char *ptr;

	if(!path)
		return false;

	length = (int)strlen(path);
	ptr = (char *)path + (length - 1);

	//loop from the end of the path looking for a '.'
	while((ptr != path) && (*ptr != '.'))
		ptr--;

	if(ptr == path)
		return false;			//no extension found

	ptr++;

	strncpy(extension, ptr, len - 1);

	return true;
}

/*
----------------
Sys_GetFileExtension

- the extension is stored in the file handle structure
- handle:		open file handle to get the path of
- return:		pointer to the extension string
----------------
*/
char *Sys_GetFileExtension(const void *handle)
{
	sysfile_handle_t *h;
	int i;
	char *sptr, *eptr, tempc;

	if(!handle)
		return NULL;

	h = (sysfile_handle_t *)handle;
	sptr = h->path;
	if(*sptr == '\0')
	{
		h->ext[0] = '\0';
		return h->ext;
	}

	i = 0;
	do	//while(*sptr != '\0')
	{
		i++;
		sptr++;
		if(i > MAX_PATH)
		{
			*sptr = '\0';		//add the missing '\0' at the end of the string array
			break;
		}
	} while(*sptr != '\0');

	sptr--;						//point to character just before the '\0'

								//loop from the end of the path looking for a '.'
	i = 0;
	while((sptr != h->path) && (*sptr != '.'))
	{
		h->ext[i] = *sptr;
		sptr--;
		i++;
		if(i >= MAX_EXTENSION_LENGTH)
		{
			h->ext[0] = '\0';
			return h->ext;		//either the extension is longer than MAX_EXTENSION_LENGTH or doesn't exist
		}
	}

	if(sptr == h->path)
	{
		h->ext[0] = '\0';
		return h->ext;			//no extension found
	}

	eptr = &h->ext[i - 1];
	sptr = h->ext;
	REVERSE_ARRAY(sptr, eptr, tempc);

	h->ext[i] = '\0';
	return h->ext;
}

/*
----------------
Sys_OpenFile

- gets the file extension if there is one
- fname:		file path and name to open
- fm:			open method
- return:		file handle
----------------
*/
void *Sys_OpenFile(const char *fname, file_mode_e fm)
{
	sysfile_handle_t *handle;

	if(!(handle = (sysfile_handle_t *)calloc(1, sizeof(sysfile_handle_t))))
		return NULL;

	switch(fm)
	{
		case FM_READ:
			if((handle->syshandle = CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
				return NULL;

			break;

		case FM_WRITE:
			if((handle->syshandle = CreateFileA(fname, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
				return NULL;

			break;

		case FM_APPEND:
			if((handle->syshandle = CreateFileA(fname, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
				return NULL;

			break;

		case FM_READ_AND_WRITE:
			if((handle->syshandle = CreateFileA(fname, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
				return NULL;

			break;

		case FM_READ_AND_APPEND:
			if((handle->syshandle = CreateFileA(fname, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
				return NULL;

			break;

		case FM_CREATE_WRITE:
			if((handle->syshandle = CreateFileA(fname, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
				return NULL;

			break;

		case FM_CREATE_READ_AND_WRITE:
			if((handle->syshandle = CreateFileA(fname, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
				return NULL;

			break;

		default:
			return NULL;
	}

	strncpy(handle->path, fname, MAX_PATH);
	handle->path[MAX_PATH - 1] = '\0';		//strncpy doesn't necessarily write a '\0' if MAX_PATH was reached
	Sys_GetFileExtension((void *)handle);
	return (void *)handle;
}

/*
----------------
Sys_CloseFile

- handle:		file handle to close
----------------
*/
void Sys_CloseFile(const void *handle)
{
	if(handle)
	{
		CloseHandle((HANDLE)((sysfile_handle_t *)handle)->syshandle);
		free((void *)handle);
	}
}

/*
----------------
Sys_FileLength

- handle:		get the length of the file
- return:		length in bytes
----------------
*/
long long Sys_GetFileLength(const void *handle)
{
	long long size;

	if(!handle)
		return -1;

	if(!GetFileSizeEx((HANDLE)((sysfile_handle_t *)handle)->syshandle, (LARGE_INTEGER *)&size))
		return -1;

	return size;
}

/*
----------------
Sys_ReadFile

- handle:		open file handle
- size:			number of bytes to read from the file
- data:			pre-allocated buffer to store the data
- return:		number of bytes read
----------------
*/
dword Sys_ReadFile(const void *handle, dword size, byte *data)
{
	dword ret;

	if(!ReadFile((HANDLE)((sysfile_handle_t *)handle)->syshandle, (void *)data, size, &ret, NULL))
		return 0;

	return ret;
}

/*
----------------
Sys_ListFiles

- path:			path where the search begins
- ext:			limit the search to a certain file extension
- num_files:	returns the number of files found
- subdirs:		true = recurse through subdirectories
- return:		list of files found
----------------
*/
//NOTE: num_files must be zeroed before calling this function
filelist_t *Sys_ListFiles(const char *path, const char *ext, int *num_files, const bool subdirs)
{
	filelist_t *fl = NULL, *fcur = NULL, *fnew;
	struct _finddata_t info;
	int ret, handle;
	char npath[MAX_PATH];
	char fext[MAX_EXTENSION_LENGTH];

	if(!num_files)
		return NULL;

	*num_files = 0;
	ret = snprintf(npath, sizeof(npath) - 1, "%s\\*", path);
	if(ret <= 0 || ret >= sizeof(npath) - 1)
		return NULL;

	if((handle = (int)_findfirst(npath, &info)) == -1)
		return NULL;

	do
	{
		if((info.attrib & _A_SUBDIR) && subdirs)
		{
			if(strcmp(info.name, ".") && strcmp(info.name, ".."))
			{
				if(!(fnew = (filelist_t *)calloc(1, sizeof(filelist_t))))
					Sys_Error("Sys_ListFiles: Failed to allocate a file list element");

				if(fl)
					fcur->next = fnew;
				else
					fl = fnew;

				fcur = fnew;

				ret = snprintf(npath, sizeof(npath) - 1, "%s\\%s", path, info.name);
				if(ret <= 0 || ret >= sizeof(npath) - 1)
					return NULL;

				strcpy(fnew->name, npath);
				fnew->subdir = Sys_ListFiles(npath, ext, num_files, subdirs);
			}
		}
		else
		{
			if(ext)
			{
				if(Sys_GetFilePathExtension(info.name, fext, sizeof(fext)))
				{
					if(!_strnicmp(fext, ext, sizeof(fext)))
					{
						if(!(fnew = calloc(1, sizeof(filelist_t))))
							Sys_Error("Sys_ListFiles: Failed to allocate a file list element");

						if(fl)
							fcur->next = fnew;
						else
							fl = fnew;

						fcur = fnew;

						ret = snprintf(npath, sizeof(npath) - 1, "%s\\%s", path, info.name);
						if(ret <= 0 || ret >= sizeof(npath) - 1)
							return NULL;

						strcpy(fnew->name, npath);
						(*num_files)++;
					}
				}
			}
			else
			{
				if(!(fnew = (filelist_t *)calloc(1, sizeof(filelist_t))))
					Sys_Error("Sys_ListFiles: Failed to allocate a file list element");

				if(fl)
					fcur->next = fnew;
				else
					fl = fnew;

				fcur = fnew;

				snprintf(npath, sizeof(npath) - 1, "%s\\%s", path, info.name);
				if(ret <= 0 || ret >= sizeof(npath) - 1)
					return NULL;

				strcpy(fnew->name, npath);
				(*num_files)++;
			}
		}
	} while(_findnext(handle, &info) != -1);

	_findclose(handle);
	return fl;
}

/*
----------------
Sys_DeleteFileList

- fl:			previously created file list
----------------
*/
void Sys_DeleteFileList(filelist_t *fl)
{
	filelist_t *pfl, *curfl;

	curfl = fl;
	while(curfl)
	{
		if(curfl->subdir)
			Sys_DeleteFileList(curfl->subdir);

		pfl = curfl->next;
		free(curfl);
		curfl = pfl;
	}
}

/*
----------------
Sys_Thread

- starting point for each thread
- lpParam:		custom parameter sent into the thread starting function
- return:		thread exit value
----------------
*/
DWORD WINAPI Sys_Thread(LPVOID lpParam)
{
	return (DWORD)ThreadMain(lpParam);
}

/*
----------------
Sys_CreateThreads

- num:			number of threads to create
- return:		true if success, false otherwise
----------------
*/
bool Sys_CreateThreads(const dword num)
{
	dword i;
	DWORD threadid;

	Sys_Log("Creating %d threads", num);
	if(!(si.hThreads = (HANDLE *)calloc(num, sizeof(HANDLE))))
		Sys_Error("Sys_CreateThreads: Failed to allocate %d thread handles", num);

	for(i = 0; i < num; i++)
	{
		if(!(si.hThreads[i] = CreateThread(NULL, 0, Sys_Thread, (LPVOID)i, 0, &threadid)))
		{
			Sys_TerminateThreads(i);
			free(si.hThreads);
			si.hThreads = NULL;
			return false;
		}
	}

	Sys_Log("%d threads running total", num + 1);
	return true;
}

/*
----------------
Sys_CreateSignal

- return:		created signal
----------------
*/
void *Sys_CreateSignal(void)
{
	return CreateEvent(NULL, TRUE, FALSE, NULL);
}

/*
----------------
Sys_CreateMutex

- return:		created mutex
----------------
*/
void *Sys_CreateMutex(void)
{
	CRITICAL_SECTION *cs;

	if(!(cs = (CRITICAL_SECTION *)malloc(sizeof(CRITICAL_SECTION))))
		return NULL;

	InitializeCriticalSection(cs);

	return (void *)cs;
}

/*
----------------
Sys_LockMutex

- mutex:		mutex to lock
----------------
*/
void Sys_LockMutex(void *mutex)
{
	if(mutex)
		EnterCriticalSection((CRITICAL_SECTION *)mutex);
}

/*
----------------
Sys_UnlockMutex

- mutex:		mutex to unlock
----------------
*/
void Sys_UnlockMutex(void *mutex)
{
	if(mutex)
		LeaveCriticalSection((CRITICAL_SECTION *)mutex);
}

/*
----------------
Sys_SignalThread

- signal:		signal to set
----------------
*/
void Sys_SignalThread(void *signal)
{
	if(signal)
		SetEvent((HANDLE)signal);
}

/*
----------------
Sys_BlockThread

- signal:		signal to block on
----------------
*/
void Sys_BlockThread(void *signal, void *mutex)
{
	ResetEvent((HANDLE)signal);
	WaitForSingleObject((HANDLE)signal, INFINITE);
}

/*
----------------
Sys_DestroyMutex

- mutex:		mutex handle to delete
----------------
*/
void Sys_DestroyMutex(void *mutex)
{
	if(!mutex)
		return;

	DeleteCriticalSection((CRITICAL_SECTION *)mutex);
	free(mutex);
	mutex = NULL;
}

/*
----------------
Sys_DestroySignal

- signal:		signal handle to be closed
----------------
*/
void Sys_DestroySignal(void *signal)
{
	if(signal)
		CloseHandle((HANDLE)signal);
}

/*
----------------
Sys_ExitThread

- value:		exit value
----------------
*/
void Sys_ExitThread(const int value)
{
	ExitThread(value);
}

/*
----------------
Sys_TerminateThreads

- num:			number of threads to terminate
----------------
*/
void Sys_TerminateThreads(const dword num)
{
	dword i;

	Sys_Log("Terminating %d threads", num);
	for(i = 0; i < num; i++)
	{
		if(si.hThreads[i])
		{
			if(WaitForSingleObject(si.hThreads[i], 2000) == WAIT_TIMEOUT)
				TerminateThread(si.hThreads[i], -1);

			CloseHandle(si.hThreads[i]);
		}
	}

	free(si.hThreads);
	si.hThreads = NULL;

	//do this here to make sure the threads are terminated before destroying the mutex
	if(logmutex)
		Sys_DestroyMutex(logmutex);
}

/*
----------------
Sys_ReadToken

- token:		storage for the found token
- line:			pointer that steps through the current line
- size:			size of the token buffer in bytes
- return:		true = a token was found,
				false = EOL or EOF
----------------
*/
bool Sys_ReadToken(char *token, char **line, const dword size)
{
	char *ptr;
	dword i = 0;

	token[0] = '\0';
	if(!line)
		return false;
	else if(!(*line))
		return false;

	ptr = token;

	//scan off leading spaces and tabs
	while((**line == ' ') || (**line == '\t') || (**line == '\r') || (**line == '\n'))
	{
		if(**line == '\n' || **line == '\r')
			return false;

		(*line)++;
	}

	//parse the token
	while((**line != ' ') && (**line != '\t') && (**line != '\r') && (**line != '\n'))
	{
		if(!(**line))
			break;

		if(i >= MAX_TOKEN_LENGTH - 1)
			break;

		*ptr = **line;
		ptr++;
		i++;
		(*line)++;
	}

	*ptr = '\0';
	return true;
}

/*
----------------
Sys_ReadIniFile

- sets defaults for any values not in the ini file
----------------
*/
void Sys_ReadIniFile(void)
{
	char line[256];
	char token[MAX_TOKEN_LENGTH];
	char *ptr;
	FILE *fp;

	//defaults
	si.width = 1024;
	si.height = 768;
	si.colordepth = 32;
	si.zbufferdepth = 24;
	si.antialiasing = 0;
	si.num_threads = 0;

	ci.L = C_L;
	ci.h = C_H;
	ci.rho = C_RHO;
	ci.Re = C_RE;
	ci.numxc = NUM_XCELLS;
	ci.numyc = NUM_YCELLS;
	ci.delt = DELTAT;
	ci.oma = OMEGA;
	ci.omu = OMEGAU;
	ci.omv = OMEGAV;
	ci.omp = OMEGAP;
	ci.sset = SS_EPSILONT;
	ci.sses = SS_EPSILONS;
	ci.ssep = SS_EPSILONP;
	ci.animscale = R_ANIMSCALE;

	if(!(fp = fopen(INIFILE_NAME, "r")))
		Sys_Error("ReadIniFile: Failed to open %s", INIFILE_NAME);

	while(ptr = fgets(line, sizeof(line), fp))
	{
		if(!Sys_ReadToken(token, &ptr, sizeof(token)))
			continue;

		if(!_stricmp(token, "fullscreen"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			if(!_stricmp(token, "true"))
				si.fullscreen = true;
			else
				si.fullscreen = false;
		}
		else if(!_stricmp(token, "width"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			si.width = (int)strtol(token, NULL, 10);
		}
		else if(!_stricmp(token, "height"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			si.height = (int)strtol(token, NULL, 10);
		}
		else if(!_stricmp(token, "colordepth"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			si.colordepth = (int)strtol(token, NULL, 10);
		}
		else if(!_stricmp(token, "zbufferdepth"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			si.zbufferdepth = (int)strtol(token, NULL, 10);
		}
		else if(!_stricmp(token, "antialiasing"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			si.antialiasing = (int)strtol(token, NULL, 10);
		}
		else if(!_stricmp(token, "threads"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			if(!_stricmp(token, "detect"))
				si.num_threads = 0;
			else
				si.num_threads = (int)strtol(token, NULL, 10);
		}
		else if(!_stricmp(token, "cpuid_dump"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			si.dumpcpuid = (int)strtol(token, NULL, 10);
		}
		else if(!_stricmp(token, "r_path"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			if(!_stricmp(token, "DETECT"))
				si.r_path = R_DETECT;
			else if(!_stricmp(token, "GENERIC"))
				si.r_path = R_GENERIC;
			else if(!_stricmp(token, "ASM"))
				si.r_path = R_ASM;
			else
				si.r_path = R_DETECT;
		}
		else if(!_stricmp(token, "L"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			ci.L = strtod(token, NULL);
		}
		else if(!_stricmp(token, "h"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			ci.h = strtod(token, NULL);
		}
		else if(!_stricmp(token, "rho"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			ci.rho = strtod(token, NULL);
		}
		else if(!_stricmp(token, "Re"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			ci.Re = strtod(token, NULL);
		}
		else if(!_stricmp(token, "nx"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			ci.numxc = (int)strtol(token, NULL, 10);
		}
		else if(!_stricmp(token, "ny"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			ci.numyc = (int)strtol(token, NULL, 10);
		}
		else if(!_stricmp(token, "dt"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			ci.delt = strtod(token, NULL);
		}
		else if(!_stricmp(token, "omegaa"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			ci.oma = strtod(token, NULL);
		}
		else if(!_stricmp(token, "omegau"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			ci.omu = strtod(token, NULL);
		}
		else if(!_stricmp(token, "omegav"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			ci.omv = strtod(token, NULL);
		}
		else if(!_stricmp(token, "omegap"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			ci.omp = strtod(token, NULL);
		}
		else if(!_stricmp(token, "sset"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			ci.sset = strtod(token, NULL);
		}
		else if(!_stricmp(token, "sses"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			ci.sses = strtod(token, NULL);
		}
		else if(!_stricmp(token, "ssep"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			ci.ssep = strtod(token, NULL);
		}
		else if(!_stricmp(token, "animscale"))
		{
			Sys_ReadToken(token, &ptr, sizeof(token));
			ci.animscale = (float)strtod(token, NULL);
		}
	}
	
	fclose(fp);
}

/*
----------------
In_Init
----------------
*/
void In_Init(void)
{
#ifdef INPUT_USE_DIRECTINPUTKEYBOARD
	HRESULT dirval;
	DIPROPDWORD dipdw2 =
	{
		{
			sizeof(DIPROPDWORD),
			sizeof(DIPROPHEADER),
			0,
			DIPH_DEVICE,
		},
		DIK_BUFFERSIZE,
	};

	if(!hInstDInput)
	{
		if(!(hInstDInput = LoadLibrary("dinput8.dll")))
			Sys_Error("In_Init: Failed to load dinput8.dll");
	}

	if(!gDirectInput8Create)
	{
		if(!(gDirectInput8Create = (HRESULT (WINAPI *)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter))GetProcAddress(hInstDInput, "DirectInput8Create")))
			Sys_Error("In_Init: Failed to get DirectInput8Create process address");
	}

	if((dirval = gDirectInput8Create(si.g_hInstance, DIRECTINPUT_VERSION, &G_DirectInput8, (void **)&lpDirectInput, NULL)) != DI_OK)
		Sys_Error("In_Init: Failed to create DirecInput root object");

	//initialize the keyboard
	if((dirval = lpDirectInput->lpVtbl->CreateDevice(lpDirectInput, &GUID_SysKeyboard, &lpDIKeyboard, NULL)) != DI_OK)
		Sys_Error("In_Init: Failed to create DirectInput keyboard device");

	if((dirval = lpDIKeyboard->lpVtbl->SetDataFormat(lpDIKeyboard, &keyboard_df)) != DI_OK)
		Sys_Error("In_Init: Failed to set DirectInput keyboard data format");

	if((dirval = lpDIKeyboard->lpVtbl->SetCooperativeLevel(lpDIKeyboard, si.g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)) != DI_OK)
		Sys_Error("In_Init: Failed to set DirectInput keyboard cooperative level");

	if((dirval = lpDIKeyboard->lpVtbl->SetProperty(lpDIKeyboard, DIPROP_BUFFERSIZE, &dipdw2.diph)) != DI_OK)
		Sys_Error("In_Init: Failed to set the DirectInput keyboard buffer properties");

	si.keyboardacquired = true;
#endif
}

/*
----------------
In_ReleaseKeyboard
----------------
*/
void In_ReleaseKeyboard(void)
{
#ifdef INPUT_USE_DIRECTINPUTKEYBOARD
	lpDIKeyboard->lpVtbl->Unacquire(lpDIKeyboard);
	si.keyboardacquired = false;
#endif
}

/*
----------------
In_AcquireKeyboard
----------------
*/
void In_AcquireKeyboard(void)
{
#ifdef INPUT_USE_DIRECTINPUTKEYBOARD
	HRESULT dirval;

	if((dirval = lpDIKeyboard->lpVtbl->Acquire(lpDIKeyboard)) != DI_OK)
		Sys_Error("In_AcquireMouse: Failed to acquire the DirectInput keyboard");

	si.keyboardacquired = true;
#endif
}

/*
----------------
In_TranslateKey

- translate DirectInput key enums into Win32 key enums
- key:			DirectInput key values

- return:		Win32 equivalent virtual key values
----------------
*/
int In_TranslateKey(const int key)
{
	switch(key)
	{
		case DIK_F11:
			return 0x54;

		case DIK_F12:
			return 0x55;

		case DIK_NUMPADENTER:
			return 0x56;

		case DIK_RCONTROL:
			return 0x57;

		case DIK_VOLUMEDOWN:
			return 0x58;

		case DIK_VOLUMEUP:
			return 0x59;

		case DIK_DIVIDE:
			return 0x5A;

		case DIK_SYSRQ:
			return 0x5B;

		case DIK_RMENU:
			return 0x5C;

		case DIK_PAUSE:
			return 0x5D;

		case DIK_HOME:
			return 0x5E;

		case DIK_UP:
			return 0x5F;

		case DIK_PRIOR:
			return 0x60;

		case DIK_LEFT:
			return 0x61;

		case DIK_RIGHT:
			return 0x62;

		case DIK_END:
			return 0x63;

		case DIK_DOWN:
			return 0x64;

		case DIK_NEXT:
			return 0x65;

		case DIK_INSERT:
			return 0x66;

		case DIK_DELETE:
			return 0x67;
	}

	return key;
}

/*
----------------
In_UpdateKeyboard
----------------
*/
void In_UpdateKeyboard(void)
{
#ifdef INPUT_USE_DIRECTINPUTKEYBOARD
	DIDEVICEOBJECTDATA diod;
	HRESULT dirval;
	DWORD elements;
	unsigned int i;

	if(!si.keyboardacquired)
		return;

	for(i = 0; i < 1024; i++)
	{
		elements = 1;
		if((dirval = lpDIKeyboard->lpVtbl->GetDeviceData(lpDIKeyboard, sizeof(DIDEVICEOBJECTDATA), &diod, &elements, 0)) != DI_OK)
		{
			if(dirval == DIERR_NOTACQUIRED)
			{
				if((dirval = lpDIKeyboard->lpVtbl->Acquire(lpDIKeyboard)) != DI_OK)
				{
					Sys_Error("In_UpdateKeyboard: Failed to acquire Direct Input keyboard: %d", dirval);
					break;
				}

				continue;
			}
		}

		if((dirval != DI_OK) || !elements)
			break;

		if(diod.dwData & 0x80)
			si.keyevents[In_TranslateKey(diod.dwOfs)] = true;
		else
			si.keyevents[In_TranslateKey(diod.dwOfs)] = false;
	}
#endif
}

/*
----------------
In_Shutdown
----------------
*/
void In_Shutdown(void)
{
	if(lpDirectInput)
	{
#ifdef INPUT_USE_DIRECTINPUTKEYBOARD
		if(lpDIKeyboard)
		{
			lpDIKeyboard->lpVtbl->Unacquire(lpDIKeyboard);
			lpDIKeyboard->lpVtbl->Release(lpDIKeyboard);
			lpDIKeyboard = NULL;
		}
#endif
		lpDirectInput->lpVtbl->Release(lpDirectInput);
		lpDirectInput = NULL;
	}

	if(hInstDInput)
	{
		FreeLibrary(hInstDInput);
		hInstDInput = NULL;
		gDirectInput8Create = NULL;
	}
}

/*
----------------
Sys_SetFullscreenVideo

- w:			width in pixels
- h:			height in pixels
- colordepth:	number of bits-per-pixel
----------------
*/
void Sys_SetFullscreenVideo(const dword w, const dword h, const dword colordepth)
{
	HRESULT ddrval;

	if(!hInstDDraw)
	{
		if(!(hInstDDraw = LoadLibrary("ddraw.dll")))
			Sys_Error("Sys_SetFullscreenVideo: Failed to load ddraw.dll");
	}

	if(!gDirectDrawCreateEx)
	{
		if(!(gDirectDrawCreateEx = (HRESULT (WINAPI *)(GUID *lpGuid, LPVOID *lplpDD, REFIID iid, IUnknown *pUnkOuter))GetProcAddress(hInstDDraw, "DirectDrawCreateEx")))
			Sys_Error("Sys_SetFullscreenVideo: Failed to get DirectDrawCreateEx process address");
	}

	if((ddrval = gDirectDrawCreateEx(NULL, (void **)&lpDirectDraw, &IID_IDirectDraw7, NULL)) != DD_OK)
		Sys_Error("Sys_SetFullscreenVideo: Failed to create DirectDraw root object");

	if((ddrval = lpDirectDraw->lpVtbl->SetCooperativeLevel(lpDirectDraw, si.g_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT)) != DD_OK)
		Sys_Error("Sys_SetFullscreenVideo: Failed to set DirectDraw cooperative level");

	if((ddrval = lpDirectDraw->lpVtbl->SetDisplayMode(lpDirectDraw, w, h, colordepth, 0, 0)) != DD_OK)
		Sys_Error("Sys_SetFullscreenVideo: Failed to set fullscreen video mode");

	while(ShowCursor(FALSE) >= 0)
		;
}

/*
----------------
GL_ExtensionsEnum

- called for every OpenGL extension loaded

- name:			extension name
- extension:	extension enum value
- fp:			pointer to FILE for logging the extensions loaded
- return:		GLENUMERATION_TRUE continues loading the extensions,
				GLENUMERATION_FALSE stops the loading
----------------
*/
static int GL_ExtensionsEnum(const char *name, const gl_extensions_t extension, void *fp)
{
	si.glextensions[extension] = true;
	fprintf((FILE *)fp, "%s\n", name);

	return GLENUMERATION_TRUE;
}

/*
----------------
Sys_VideoInit
----------------
*/
void Sys_VideoInit(void)
{
	WNDCLASS wclass;
	RECT rect;
	int w, h, x, y;
	int wndstyle, wndtype;
	FILE *fp;
	dword i;
	int pixelformat;
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),	//size of this pfd
		1,								//version number
		PFD_DRAW_TO_WINDOW |			//support window
		PFD_SUPPORT_OPENGL |			//support OpenGL
		PFD_DOUBLEBUFFER,				//double buffered
		PFD_TYPE_RGBA,					//RGBA type
		32,								//color depth (set to bitdepth)
		0, 0, 0, 0, 0, 0,				//color bits ignored
		0,								//no alpha buffer
		0,								//shift bit ignored
		0,								//no accumulation buffer
		0, 0, 0, 0, 					//accum bits ignored
		24,								//z-buffer depth (set to zbufferdepth)
		0,								//stencil buffer
		0,								//no auxiliary buffer
		PFD_MAIN_PLANE,					//main layer
		0,								//reserved
		0, 0, 0							//layer masks ignored
	};

	unsigned int count = 0;
	int attr[22];

	wclass.style			= CS_OWNDC;
	wclass.lpfnWndProc		= (WNDPROC)WndProc;
	wclass.cbClsExtra		= 0;
	wclass.cbWndExtra		= 0;
	wclass.hInstance		= si.g_hInstance;
	wclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wclass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wclass.hbrBackground	= (void *)COLOR_MENU;
	wclass.lpszMenuName		= 0;
	wclass.lpszClassName	= PROJECT_NAME;

	if(!RegisterClass(&wclass))
		Sys_Error("Sys_VideoInit: Failed to register window class");

	rect.left = rect.top = 0;
	rect.right = si.width;
	rect.bottom = si.height;
	wndstyle = WS_OVERLAPPED;
	wndtype = WS_EX_CLIENTEDGE;

	AdjustWindowRect(&rect, wndstyle, FALSE);

	w = h = x = y = 0;
	if(!(si.g_hWnd = CreateWindowEx(wndtype, PROJECT_NAME, PROJECT_NAME, wndstyle, x, y, w, h, NULL, NULL, si.g_hInstance, NULL)))
		Sys_Error("Sys_VideoInit: Failed to create the root window");

	pfd.cColorBits = si.colordepth;
	pfd.cDepthBits = si.zbufferdepth;

	if(OpenGL_Init(GLDRIVER_ANY, Sys_Message) == GLDRIVER_NONE)
		Sys_Error("Sys_VideoInit: No OpenGL acceleration available");

	if(si.hDC)
		Sys_Error("Sys_VideoInit: Non-NULL hDC");

	if(!(si.hDC = GetDC(si.g_hWnd)))
		Sys_Error("Sys_VideoInit: Failed to get the device context");

	if(!(pixelformat = gwglChoosePixelFormat(si.hDC, &pfd)))
		Sys_Error("Sys_VideoInit: Unable to choose pixel format");

	if(!gwglDescribePixelFormat(si.hDC, pixelformat, sizeof(pfd), &pfd))
		Sys_Error("Sys_VideoInit: Failed to describe pixel format");

	if(!gwglSetPixelFormat(si.hDC, pixelformat, &pfd))
		Sys_Error("Sys_VideoInit: Unable to set pixel format");

	if(!(si.hRC = gwglCreateContext(si.hDC)))
		Sys_Error("Sys_VideoInit: Failed to create OpenGL subsystem context");

	if(!gwglMakeCurrent(si.hDC, si.hRC))
		Sys_Error("Sys_VideoInit: Failed to make OpenGL subsystem current");

	OpenGL_EnableExtensions(si.hDC, NULL, NULL);

//glextensions[GWGL_ARB_pixel_format] = 0;
	if(si.glextensions[GWGL_ARB_pixel_format])
	{
		attr[0] = WGL_SUPPORT_OPENGL_ARB;
		attr[1] = GL_TRUE;
		attr[2] = WGL_ACCELERATION_ARB;
		attr[3] = WGL_FULL_ACCELERATION_ARB;
		attr[4] = WGL_DRAW_TO_WINDOW_ARB;
		attr[5] = GL_TRUE;
		attr[6] = WGL_PIXEL_TYPE_ARB;
		attr[7] = WGL_TYPE_RGBA_ARB;
		attr[8] = WGL_COLOR_BITS_ARB;
		attr[9] = si.colordepth;
		attr[10] = WGL_DEPTH_BITS_ARB;
		attr[11] = si.zbufferdepth;
		attr[12] = WGL_STENCIL_BITS_ARB;
		attr[13] = 0;
		attr[14] = WGL_DOUBLE_BUFFER_ARB;
		attr[15] = GL_TRUE;
		if(!si.antialiasing)
		{
			attr[16] = 0;
			attr[17] = 0;
			attr[18] = 0;
			attr[19] = 0;
			attr[20] = 0;
			attr[21] = 0;
		}
		else
		{
			if(si.glextensions[GWGL_ARB_multisample])
			{
				attr[16] = WGL_SAMPLE_BUFFERS_ARB;
				attr[17] = GL_TRUE;
				attr[18] = WGL_SAMPLES_ARB;
				attr[19] = si.antialiasing;
				attr[20] = 0;
				attr[21] = 0;
			}
			else
			{
				attr[16] = 0;
				attr[17] = 0;
				attr[18] = 0;
				attr[19] = 0;
				attr[20] = 0;
				attr[21] = 0;
			}
		}

		pixelformat = 0;
		if(!gwglChoosePixelFormatARB(si.hDC, (const int *)attr, NULL, 1, &pixelformat, &count) || !count)
			Sys_Error("Sys_VideoInit: Unable to choose wgl pixel format");

		if(si.hRC)
		{
			gwglMakeCurrent(NULL, NULL);
			gwglDeleteContext(si.hRC);
			si.hRC = NULL;
		}

		if(si.hDC)
		{
			ReleaseDC(si.g_hWnd, si.hDC);
			si.hDC = NULL;
		}

		if(si.g_hWnd)
			DestroyWindow(si.g_hWnd);

		UnregisterClass(PROJECT_NAME, si.g_hInstance);

		if(si.fullscreen)
		{
			wndstyle = WS_POPUP | WS_VISIBLE;
			wndtype = WS_EX_TOPMOST;
		}
		else
		{
			wndstyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
			wndtype = WS_EX_CLIENTEDGE;
		}

		if(si.fullscreen)
		{
			wclass.style			= 0;
			wclass.lpfnWndProc		= (WNDPROC)WndProc;
			wclass.cbClsExtra		= 0;
			wclass.cbWndExtra		= 0;
			wclass.hInstance		= si.g_hInstance;
			wclass.hIcon			= 0;
			wclass.hCursor			= LoadCursor(NULL, IDC_ARROW);
			wclass.hbrBackground	= (void *)COLOR_MENU;
			wclass.lpszMenuName		= 0;
			wclass.lpszClassName	= PROJECT_NAME;
		}
		else
		{
			wclass.style			= CS_OWNDC;
			wclass.lpfnWndProc		= (WNDPROC)WndProc;
			wclass.cbClsExtra		= 0;
			wclass.cbWndExtra		= 0;
			wclass.hInstance		= si.g_hInstance;
			wclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
			wclass.hCursor			= LoadCursor(NULL, IDC_ARROW);
			wclass.hbrBackground	= (void *)COLOR_MENU;
			wclass.lpszMenuName		= 0;
			wclass.lpszClassName	= PROJECT_NAME;
		}

		if(!RegisterClass(&wclass))
			Sys_Error("Sys_VideoInit: Failed to register window class");

		rect.left = rect.top = 0;
		rect.right = si.width;
		rect.bottom = si.height;

		AdjustWindowRect(&rect, wndstyle, FALSE);

		if(!si.fullscreen)
		{
			w = rect.right - rect.left;
			h = rect.bottom - rect.top;
			x = (GetSystemMetrics(SM_CXSCREEN) - si.width) / 2;
			y = (GetSystemMetrics(SM_CYSCREEN) - si.height) / 2;
		}
		else
			w = h = x = y = 0;

		if(!(si.g_hWnd = CreateWindowEx(wndtype, PROJECT_NAME, PROJECT_NAME, wndstyle, x, y, w, h, NULL, NULL, si.g_hInstance, NULL)))
			Sys_Error("Sys_VideoInit: Failed to create the root window");

		ShowWindow(si.g_hWnd, SW_SHOW);
		UpdateWindow(si.g_hWnd);

		if(si.fullscreen)
			Sys_SetFullscreenVideo(si.width, si.height, si.colordepth);

		SetForegroundWindow(si.g_hWnd);
		SetFocus(si.g_hWnd);

		if(!(si.hDC = GetDC(si.g_hWnd)))
			Sys_Error("Sys_VideoInit: Failed to get DC");

		if(!gwglDescribePixelFormat(si.hDC, pixelformat, sizeof(pfd), &pfd))
			Sys_Error("Sys_VideoInit: Failed to describe pixel format");

		if(!gwglSetPixelFormat(si.hDC, pixelformat, &pfd))
			Sys_Error("Sys_VideoInit: Unable to set wgl pixel format");

		if(!(si.hRC = gwglCreateContext(si.hDC)))
			Sys_Error("Sys_VideoInit: Failed to create OpenGL subsystem context");

   		if(!gwglMakeCurrent(si.hDC, si.hRC))
			Sys_Error("Sys_VideoInit: Failed to make OpenGL subsystem current");

		if(!(fp = fopen("extensions.txt", "w")))
			Sys_Error("Sys_VideoInit: Failed to open file extensions.txt");

		fprintf(fp, "EnableOpenGL:\n\n");
		fprintf(fp, "%s\n", gglGetString(GL_VENDOR));
		fprintf(fp, "%s\n", gglGetString(GL_RENDERER));
		fprintf(fp, "OpenGL %s\n", gglGetString(GL_VERSION));
//		fprintf(fp, "%s\n\n", gglGetString(GL_EXTENSIONS));

		for(i = 0; i < GGL_MAX_EXTENSIONS; i++)
			si.glextensions[i] = false;

		OpenGL_EnableExtensions(si.hDC, GL_ExtensionsEnum, fp);
		fclose(fp);

		if(si.antialiasing && si.glextensions[GGL_ARB_multisample])
			gglEnable(GL_MULTISAMPLE_ARB);
	}
	else
	{
		if(si.hRC)
		{
			gwglMakeCurrent(NULL, NULL);
			gwglDeleteContext(si.hRC);
			si.hRC = NULL;
		}

		if(si.hDC)
		{
			ReleaseDC(si.g_hWnd, si.hDC);
			si.hDC = NULL;
		}

		if(si.g_hWnd)
			DestroyWindow(si.g_hWnd);

		UnregisterClass(PROJECT_NAME, si.g_hInstance);

		if(si.fullscreen)
		{
			wndstyle = WS_POPUP | WS_VISIBLE;
			wndtype = WS_EX_TOPMOST;
		}
		else
		{
			wndstyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
			wndtype = WS_EX_CLIENTEDGE;
		}

		if(si.fullscreen)
		{
			wclass.style			= 0;
			wclass.lpfnWndProc		= (WNDPROC)WndProc;
			wclass.cbClsExtra		= 0;
			wclass.cbWndExtra		= 0;
			wclass.hInstance		= si.g_hInstance;
			wclass.hIcon			= 0;
			wclass.hCursor			= LoadCursor(NULL, IDC_ARROW);
			wclass.hbrBackground	= (void *)COLOR_MENU;
			wclass.lpszMenuName		= 0;
			wclass.lpszClassName	= PROJECT_NAME;
		}
		else
		{
			wclass.style			= CS_OWNDC;
			wclass.lpfnWndProc		= (WNDPROC)WndProc;
			wclass.cbClsExtra		= 0;
			wclass.cbWndExtra		= 0;
			wclass.hInstance		= si.g_hInstance;
			wclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
			wclass.hCursor			= LoadCursor(NULL, IDC_ARROW);
			wclass.hbrBackground	= (void *)COLOR_MENU;
			wclass.lpszMenuName		= 0;
			wclass.lpszClassName	= PROJECT_NAME;
		}

		if(!RegisterClass(&wclass))
			Sys_Error("Sys_VideoInit: Failed to register window class");

		rect.left = rect.top = 0;
		rect.right = si.width;
		rect.bottom = si.height;

		AdjustWindowRect(&rect, wndstyle, FALSE);

		w = rect.right - rect.left;
		h = rect.bottom - rect.top;
		x = (GetSystemMetrics(SM_CXSCREEN) - si.width) / 2;
		y = (GetSystemMetrics(SM_CYSCREEN) - si.height) / 2;

		if(!(si.g_hWnd = CreateWindowEx(wndtype, PROJECT_NAME, PROJECT_NAME, wndstyle, x, y, w, h, NULL, NULL, si.g_hInstance, NULL)))
			Sys_Error("Sys_VideoInit: Failed to create the root window");

		ShowWindow(si.g_hWnd, SW_SHOW);
		UpdateWindow(si.g_hWnd);

		if(si.fullscreen)
			Sys_SetFullscreenVideo(si.width, si.height, si.colordepth);

		SetForegroundWindow(si.g_hWnd);
		SetFocus(si.g_hWnd);

		pfd.cColorBits = si.colordepth;
		pfd.cDepthBits = si.zbufferdepth;

//		if(OpenGL_Init(GLDRIVER_ANY, Sys_Message) == GLDRIVER_NONE)
//			Sys_Error("Sys_VideoInit: No OpenGL acceleration available");

		if(si.hDC)
			Sys_Error("Sys_VideoInit: Non-NULL hDC");

		if(!(si.hDC = GetDC(si.g_hWnd)))
			Sys_Error("Sys_VideoInit: Failed to get DC");

		if(!(pixelformat = gwglChoosePixelFormat(si.hDC, &pfd)))
			Sys_Error("Sys_VideoInit: Unable to choose pixel format");

		if(!gwglDescribePixelFormat(si.hDC, pixelformat, sizeof(pfd), &pfd))
			Sys_Error("Sys_VideoInit: Failed to describe pixel format");

		if(!gwglSetPixelFormat(si.hDC, pixelformat, &pfd))
			Sys_Error("Sys_VideoInit: Unable to set pixel format");

		if(!(si.hRC = gwglCreateContext(si.hDC)))
			Sys_Error("Sys_VideoInit: Failed to create OpenGL subsystem context");

	    if(!gwglMakeCurrent(si.hDC, si.hRC))
			Sys_Error("Sys_VideoInit: Failed to make OpenGL subsystem current");

		if(!(fp = fopen("extensions.txt", "w")))
			Sys_Error("Sys_VideoInit: Failed to open file extensions.txt");

		fprintf(fp, "EnableOpenGL:\n\n");
		fprintf(fp, "%s\n", gglGetString(GL_VENDOR));
		fprintf(fp, "%s\n", gglGetString(GL_RENDERER));
		fprintf(fp, "OpenGL %s\n", gglGetString(GL_VERSION));
//		fprintf(fp, "%s\n\n", gglGetString(GL_EXTENSIONS));

		for(i = 0; i < GGL_MAX_EXTENSIONS; i++)
			si.glextensions[i] = false;
	
		OpenGL_EnableExtensions(si.hDC, GL_ExtensionsEnum, fp);
		fclose(fp);
	}
}

/*
----------------
Sys_VideoShutdown
----------------
*/
void Sys_VideoShutdown(void)
{
	if(si.hRC)
	{
		gwglMakeCurrent(NULL, NULL);
		gwglDeleteContext(si.hRC);
		si.hRC = NULL;
	}

	if(si.hDC)
	{
		ReleaseDC(si.g_hWnd, si.hDC);
		si.hDC = NULL;
	}

	if(lpDirectDraw)
	{
		lpDirectDraw->lpVtbl->RestoreDisplayMode(lpDirectDraw);
		lpDirectDraw->lpVtbl->SetCooperativeLevel(lpDirectDraw, si.g_hWnd, DDSCL_NORMAL);
		lpDirectDraw->lpVtbl->Release(lpDirectDraw);
		lpDirectDraw = NULL;
	}

	if(si.g_hWnd)
		DestroyWindow(si.g_hWnd);

	UnregisterClass(PROJECT_NAME, si.g_hInstance);

	while(ShowCursor(TRUE) < 0)
		;

	if(hInstDDraw)
	{
		FreeLibrary(hInstDDraw);
		hInstDDraw = NULL;
		gDirectDrawCreateEx = NULL;
	}
}

/*
----------------
Sys_UpdateScreen
----------------
*/
void Sys_UpdateScreen(void)
{
	gwglSwapBuffers(si.hDC);
}

//--------------------------------------
//BYTE ORDER SWAPPING
//--------------------------------------

short SwapShort(const short var)
{
	return (short)_byteswap_ushort((unsigned short)var);
}

short PassShort(const short var)
{
	return var;
}

int SwapInt(const int var)
{
	return (int)_byteswap_ulong((unsigned long)var);
}

int PassInt(const int var)
{
	return var;
}

float SwapFloat(const float var)
{
	unsigned long f;

	f = _byteswap_ulong(*(unsigned long *)&var);
	return *(float *)&f;
}

float PassFloat(const float var)
{
	return var;
}

/*
----------------
Sys_InitByteOrder

- sets the byte order function pointers
----------------
*/
void Sys_InitByteOrder(void)
{
	byte stest[2];

	stest[0] = 1;
	stest[1] = 0;

	if(*(short *)stest == 1)
	{
		//little-endian
		BigShort = SwapShort;
		LittleShort = PassShort;
		BigInt = SwapInt;
		LittleInt = PassInt;
		BigFloat = SwapFloat;
		LittleFloat = PassFloat;
	}
	else
	{
		//big-endian
		BigShort = PassShort;
		LittleShort = SwapShort;
		BigInt = PassInt;
		LittleInt = SwapInt;
		BigFloat = PassFloat;
		LittleFloat = SwapFloat;
	}
}

/*
----------------
WndProc
----------------
*/
LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_CLOSE:
			Sys_Shutdown();
			break;

		case WM_CREATE:
		case WM_ACTIVATE:
		case WM_DESTROY:
		case WM_PAINT:
			break;

		case WM_HOTKEY:
			return 0;

		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
				Sys_Shutdown();

#ifndef INPUT_USE_DIRECTINPUTKEYBOARD
			si.keyevents[wParam] = true;
#endif
			break;

		case WM_SYSKEYUP:
		case WM_KEYUP:
#ifndef INPUT_USE_DIRECTINPUTKEYBOARD
			si.keyevents[wParam] = false;
#endif
			break;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
			break;

		case WM_SYSCOMMAND:
			switch((wParam >> 16) & 0xFFFF)
			{
				//prevent the screen saver from taking over
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
			}

			break;

		//prevent Alt-Spaceing from halting the program
		case WM_SYSCHAR:
			return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*
 ----------------
 WinMain
 ----------------
*/
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SYSTEMTIME systime;
	MSG msg;

	if(hPrevInstance)
		return 0;

	Sys_InitByteOrder();

	memset(&si, 0, sizeof(setup_info_t));
	memset(&ci, 0, sizeof(cfd_info_t));
	si.g_hInstance = hInstance;

	Sys_Log("\n\n\n");
	Sys_Log("==========================================================");
	Sys_Log("");
	Sys_Log("CFD Internal Developing Flow\n");
	Sys_Log("© 2017 William Kimerer\n");

	GetLocalTime(&systime);
	Sys_Log("Timestamp: %d/%d/%d  %d:%d:%d.%d", systime.wMonth, systime.wDay, systime.wYear, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);

	Sys_Log("");
	Sys_Log("==========================================================\n");

	Sys_ReadIniFile();

	SetupCores();

//	if(!(logmutex = Sys_CreateMutex()))
//		Sys_Error("WinMain: Couldn't create log mutex");

	Sys_Log("\nInitializing video");
	Sys_VideoInit();

	Sys_Log("See extensions.txt for detailed video card performance logs\n");

	Sys_Log("Initializing input");
	In_Init();

	Sys_Log("\nInitializing renderer");
	Init();

	Sys_Log("\n--------------------------------------");
	Sys_Log("Initizialization complete\n");
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage(&msg, NULL, 0, 0))
				Sys_Shutdown();

			//handle or dispatch messages
			if(msg.message == WM_QUIT)
				Sys_Shutdown();
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		Render();
	}

	return TRUE;
}
