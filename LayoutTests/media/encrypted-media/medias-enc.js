const streamMedias = {
    "simpleClearKeyMSE" : {    video : {    initDataType : "cenc",
                                            mimeType     : 'video/mp4; codecs="avc1.64001F"',
                                            segments     : [ "../../content/encrypted/segments/VideoClearKeyCenc-seg-0.mp4",
                                                             "../../content/encrypted/segments/VideoClearKeyCenc-seg-1.mp4",
                                                             "../../content/encrypted/segments/VideoClearKeyCenc-seg-2.mp4",
                                                             "../../content/encrypted/segments/VideoClearKeyCenc-seg-3.mp4",
                                                           ],
                                            keys         : {    "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf" : "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf" }
                                       },
                               audio : {    initDataType : "cenc",
                                            mimeType     : 'audio/mp4; codecs="mp4a.40.2"',
                                            segments     : [ "../../content/encrypted/segments/AudioClearKeyCenc-seg-0.mp4",
                                                             "../../content/encrypted/segments/AudioClearKeyCenc-seg-1.mp4",
                                                             "../../content/encrypted/segments/AudioClearKeyCenc-seg-2.mp4",
                                                             "../../content/encrypted/segments/AudioClearKeyCenc-seg-3.mp4",
                                                           ],
                                            keys         : {   	"b0b1b2b3b4b5b6b7b8b9babbbcbdbebf" : "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf" }
                                       }
                          },
    "simpleClearKey"   :  {    video : {    initDataType : "cenc",
                                            mimeType     : 'video/mp4; codecs="avc1.64001F"',
                                            path         : "../../content/encrypted/VideoClearKeyCenc.mp4",
                                            keys         : {    "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf" : "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf" }
                                       }
                          }
                   };
