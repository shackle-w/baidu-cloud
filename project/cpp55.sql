/*
 Navicat Premium Data Transfer

 Source Server         : linux
 Source Server Type    : MySQL
 Source Server Version : 50742
 Source Host           : localhost:3306
 Source Schema         : cpp55

 Target Server Type    : MySQL
 Target Server Version : 50742
 File Encoding         : 65001

 Date: 02/06/2024 04:24:58
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for file
-- ----------------------------
DROP TABLE IF EXISTS `file`;
CREATE TABLE `file`  (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '文件和路径的id',
  `user_id` int(11) NULL DEFAULT NULL COMMENT '所属用户',
  `path` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '在当前用户下所在路径',
  `father_path_id` int(11) NULL DEFAULT NULL COMMENT '父级路径id (-1为根路径)',
  `file_type` int(11) NULL DEFAULT 0 COMMENT '文件类型: 0文件夹, 1文件',
  `file_name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '文件的名字',
  `file_hash_md5` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT '0' COMMENT '文件的md5_hash值, 如果是文件',
  `file_hash_sha1` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT '0' COMMENT '文件的sha1_hash值, 如果是文件',
  `upload_status` int(11) NULL DEFAULT 0 COMMENT '文件状态:0开始准备上传, 1上传完成',
  `delete_flag` int(11) NULL DEFAULT 0 COMMENT '文件或者文件夹, 删除标记未',
  `create_time` datetime NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
  `update_time` datetime NULL DEFAULT CURRENT_TIMESTAMP COMMENT '修改时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 85 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of file
-- ----------------------------
INSERT INTO `file` VALUES (1, 10, '/', -1, 0, NULL, '0', '0', 0, 0, '2024-05-10 14:17:20', '2024-05-10 14:17:20');
INSERT INTO `file` VALUES (2, 11, '/', -1, 0, NULL, '0', '0', 0, 0, '2024-05-10 14:18:01', '2024-05-10 14:18:01');
INSERT INTO `file` VALUES (5, 12, '/', -1, 0, NULL, '0', '0', 0, 0, '2024-05-10 21:48:11', '2024-05-10 21:48:11');
INSERT INTO `file` VALUES (64, 11, '/x/', 2, 0, 'x', '0', '0', 0, 1, '2024-05-20 01:34:18', '2024-05-20 01:34:18');
INSERT INTO `file` VALUES (65, 11, '/dir1/', 2, 0, 'dir1', '0', '0', 0, 0, '2024-05-20 01:34:21', '2024-05-20 01:34:21');
INSERT INTO `file` VALUES (66, 11, '/dir2/', 2, 0, 'dir2', '0', '0', 0, 0, '2024-05-20 01:34:25', '2024-05-20 01:34:25');
INSERT INTO `file` VALUES (67, 11, '/x/1b.txt', 64, 1, '1b.txt', 'a9cc6b37d8b03e9f358c7e35f8abf385', '6f206a591cf28e233f27b2e8d52f52b4448749a6', 1, 1, '2024-05-20 01:34:33', '2024-05-20 01:34:33');
INSERT INTO `file` VALUES (68, 11, '/dir1/1b.txt', 65, 1, '1b.txt', 'a9cc6b37d8b03e9f358c7e35f8abf385', '6f206a591cf28e233f27b2e8d52f52b4448749a6', 1, 0, '2024-05-20 01:35:21', '2024-05-20 01:35:21');
INSERT INTO `file` VALUES (69, 11, '/x/', 2, 0, 'x', '0', '0', 0, 1, '2024-05-20 01:35:26', '2024-05-20 01:35:26');
INSERT INTO `file` VALUES (70, 11, '/x/Makefile', 69, 1, 'Makefile', '7f08a59117627d0a5c3328d0efc8a364', 'f424a845ea3dfced5d8b1acc6e9384fea5cb33c1', 1, 1, '2024-05-20 01:35:37', '2024-05-20 01:35:37');
INSERT INTO `file` VALUES (71, 11, '/dir3/', 2, 0, 'dir3', '0', '0', 0, 0, '2024-05-20 10:15:28', '2024-05-20 10:15:28');
INSERT INTO `file` VALUES (72, 11, '/dir3/Makefile', 71, 1, 'Makefile', '7f08a59117627d0a5c3328d0efc8a364', 'f424a845ea3dfced5d8b1acc6e9384fea5cb33c1', 1, 1, '2024-05-20 10:15:38', '2024-05-20 10:15:38');
INSERT INTO `file` VALUES (73, 11, '/dir1/1a.txt', 65, 1, '1a.txt', 'ba1f2511fc30423bdbb183fe33f3dd0f', 'a8fdc205a9f19cc1c7507a60c4f01b13d11d7fd0', 1, 0, '2024-05-20 21:23:30', '2024-05-20 21:23:30');
INSERT INTO `file` VALUES (74, 11, '/dir2/x/', 66, 0, 'x', '0', '0', 0, 1, '2024-05-24 16:07:40', '2024-05-24 16:07:40');
INSERT INTO `file` VALUES (75, 11, '/dir2/x/', 66, 0, 'x', '0', '0', 0, 0, '2024-05-24 16:09:02', '2024-05-24 16:09:02');
INSERT INTO `file` VALUES (76, 11, '/dir2/x/client.c', 75, 1, 'client.c', '5718c8ee955e1473703d40fffe8af1b1', '0b494db8a940ddb699bab36d94c1b236b0ef7b11', 1, 1, '2024-05-24 16:09:50', '2024-05-24 16:09:50');
INSERT INTO `file` VALUES (77, 11, '/dir2/x/Makefile', 75, 1, 'Makefile', '7f08a59117627d0a5c3328d0efc8a364', 'f424a845ea3dfced5d8b1acc6e9384fea5cb33c1', 1, 0, '2024-06-02 00:23:54', '2024-06-02 00:23:54');
INSERT INTO `file` VALUES (78, 12, '/x/', 5, 0, 'x', '0', '0', 0, 0, '2024-06-02 02:50:25', '2024-06-02 02:50:25');
INSERT INTO `file` VALUES (79, 13, '/', -1, 0, NULL, '0', '0', 0, 0, '2024-06-02 03:23:00', '2024-06-02 03:23:00');
INSERT INTO `file` VALUES (80, 13, '/x/', 79, 0, 'x', '0', '0', 0, 0, '2024-06-02 03:23:08', '2024-06-02 03:23:08');
INSERT INTO `file` VALUES (81, 11, '/dir3/Makefile', 71, 1, 'Makefile', '7f08a59117627d0a5c3328d0efc8a364', 'f424a845ea3dfced5d8b1acc6e9384fea5cb33c1', 1, 1, '2024-06-02 04:11:08', '2024-06-02 04:11:08');
INSERT INTO `file` VALUES (82, 14, '/', -1, 0, NULL, '0', '0', 0, 0, '2024-06-02 04:15:46', '2024-06-02 04:15:46');
INSERT INTO `file` VALUES (83, 14, '/Makefile', 82, 1, 'Makefile', '7f08a59117627d0a5c3328d0efc8a364', 'f424a845ea3dfced5d8b1acc6e9384fea5cb33c1', 1, 0, '2024-06-02 04:16:01', '2024-06-02 04:16:01');
INSERT INTO `file` VALUES (84, 14, '/y/', 82, 0, 'y', '0', '0', 0, 0, '2024-06-02 04:16:06', '2024-06-02 04:16:06');

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user`  (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '用户id',
  `user_name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '用户名',
  `slat` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '用户盐值',
  `crypt_password` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '用户散列密码',
  `delete_tag` int(11) NULL DEFAULT 0 COMMENT '删除标记位',
  `create_time` datetime NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
  `update_time` datetime NULL DEFAULT CURRENT_TIMESTAMP COMMENT '修改时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 15 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO `user` VALUES (10, 'R', '$6$d3b83ed5$', '$6$d3b83ed5$/H9oUDjrH/s9TfZbtL4HzWAdKZee3Af1xZIn6YubSBca39OKtP.SzyGLIwcYC/h5TVxTRuvD9aiqu08qI55WM/', 0, '2024-05-10 14:17:20', '2024-05-10 14:17:20');
INSERT INTO `user` VALUES (11, 'zs', '$6$1df25334$', '$6$1df25334$swyDtyqc5nCcDM5ZbAuY8b6Yg4pobqsiNxTB5p7vy3V3DpxqQLfHNqr3ZczUO.8Vg.HuynNBLHRH3CjCQ15WO.', 0, '2024-05-10 14:18:01', '2024-05-10 14:18:01');
INSERT INTO `user` VALUES (12, 'ls', '$6$ff0bba8c$', '$6$ff0bba8c$Jn3TF6ju3z7yR/JTxzh1IFsj1cixQUbqn6ZKMdq9hc6f1fsew8cfN2ObqGkJwcdfzROKs/MTNDwF3bXbLQMCY/', 0, '2024-05-10 21:48:11', '2024-05-10 21:48:11');
INSERT INTO `user` VALUES (13, 'zl', '$6$584964e8$', '$6$584964e8$fmC1cxRImZyPDsSfmkPKxcqbr2hkgQ0OG/wu89UGvfDjdfH2UGnhkODEd0DjEn0wgmpToSktbmrpqxmI9UFhP/', 0, '2024-06-02 03:23:00', '2024-06-02 03:23:00');
INSERT INTO `user` VALUES (14, 'sq', '$6$86e41aaa$', '$6$86e41aaa$v2AXST1YEmQz3fTaLyPm52rUBzGEfNuMikSh9cDZ3N88vr96XwnOQDd6DiZgU0bVrrPEyeDw1rGaLn3HWQeiw1', 0, '2024-06-02 04:15:46', '2024-06-02 04:15:46');

SET FOREIGN_KEY_CHECKS = 1;
