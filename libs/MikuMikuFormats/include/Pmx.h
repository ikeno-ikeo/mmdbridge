#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

namespace pmx
{
	/// インデックス設定
	class PmxSetting
	{
	public:
		PmxSetting()
			: encoding(0)
			, uv(0)
			, vertex_index_size(0)
			, texture_index_size(0)
			, material_index_size(0)
			, bone_index_size(0)
			, morph_index_size(0)
			, rigidbody_index_size(0)
		{}

		/// エンコード方式
		uint8_t encoding;
		/// 追加UV数
		uint8_t uv;
		/// 頂点インデックスサイズ
		uint8_t vertex_index_size;
		/// テクスチャインデックスサイズ
		uint8_t texture_index_size;
		/// マテリアルインデックスサイズ
		uint8_t material_index_size;
		/// ボーンインデックスサイズ
		uint8_t bone_index_size;
		/// モーフインデックスサイズ
		uint8_t morph_index_size;
		/// 剛体インデックスサイズ
		uint8_t rigidbody_index_size;
		void Read(std::istream *stream);
		void Write(std::ostream *stream);
	};

	/// 頂点スキニングタイプ
	enum class PmxVertexSkinningType : uint8_t
	{
		BDEF1 = 0,
		BDEF2 = 1,
		BDEF4 = 2,
		SDEF = 3,
		QDEF = 4,
	};

	/// 頂点スキニング
	class PmxVertexSkinning
	{
	public:
		virtual void Read(std::istream *stream, PmxSetting *setting) = 0;
		virtual void Write(std::ostream *stream, PmxSetting *setting) = 0;
	};

	class PmxVertexSkinningBDEF1 : public PmxVertexSkinning
	{
	public:
		PmxVertexSkinningBDEF1()
			: bone_index(0)
		{}

		int bone_index;
		void Read(std::istream *stresam, PmxSetting *setting);
		void Write(std::ostream *stresam, PmxSetting *setting);
	};

	class PmxVertexSkinningBDEF2 : public PmxVertexSkinning
	{
	public:
		PmxVertexSkinningBDEF2()
			: bone_index1(0)
			, bone_index2(0)
			, bone_weight(0.0f)
		{}

		int bone_index1;
		int bone_index2;
		float bone_weight;
		void Read(std::istream *stresam, PmxSetting *setting);
		void Write(std::ostream *stresam, PmxSetting *setting);
	};

	class PmxVertexSkinningBDEF4 : public PmxVertexSkinning
	{
	public:
		PmxVertexSkinningBDEF4()
			: bone_index1(0)
			, bone_index2(0)
			, bone_index3(0)
			, bone_index4(0)
			, bone_weight1(0.0f)
			, bone_weight2(0.0f)
			, bone_weight3(0.0f)
			, bone_weight4(0.0f)
		{}

		int bone_index1;
		int bone_index2;
		int bone_index3;
		int bone_index4;
		float bone_weight1;
		float bone_weight2;
		float bone_weight3;
		float bone_weight4;
		void Read(std::istream *stresam, PmxSetting *setting);
		void Write(std::ostream *stresam, PmxSetting *setting);
	};

	class PmxVertexSkinningSDEF : public PmxVertexSkinning
	{
	public:
		PmxVertexSkinningSDEF()
			: bone_index1(0)
			, bone_index2(0)
			, bone_weight(0.0f)
		{
			for (int i = 0; i < 3; ++i) {
				sdef_c[i] = 0.0f;
				sdef_r0[i] = 0.0f;
				sdef_r1[i] = 0.0f;
			}
		}

		int bone_index1;
		int bone_index2;
		float bone_weight;
		float sdef_c[3];
		float sdef_r0[3];
		float sdef_r1[3];
		void Read(std::istream *stresam, PmxSetting *setting);
		void Write(std::ostream *stresam, PmxSetting *setting);
	};

	class PmxVertexSkinningQDEF : public PmxVertexSkinning
	{
	public:
		PmxVertexSkinningQDEF()
			: bone_index1(0)
			, bone_index2(0)
			, bone_index3(0)
			, bone_index4(0)
			, bone_weight1(0.0f)
			, bone_weight2(0.0f)
			, bone_weight3(0.0f)
			, bone_weight4(0.0f)
		{}

		int bone_index1;
		int bone_index2;
		int bone_index3;
		int bone_index4;
		float bone_weight1;
		float bone_weight2;
		float bone_weight3;
		float bone_weight4;
		void Read(std::istream *stresam, PmxSetting *setting);
		void Write(std::ostream *stresam, PmxSetting *setting);
	};

	/// 頂点
	class PmxVertex
	{
	public:
		PmxVertex()
			: edge(0.0f)
		{
			uv[0] = uv[1] = 0.0f;
			for (int i = 0; i < 3; ++i) {
				position[i] = 0.0f;
				normal[i] = 0.0f;
			}
			for (int i = 0; i < 4; ++i) {
				for (int k = 0; k < 4; ++k) {
					uva[i][k] = 0.0f;
				}
			}
		}

		bool operator == (const PmxVertex &v) const {
			return (std::memcmp(this, &v, sizeof(PmxVertex)) == 0);
		}

		/// 位置
		float position[3];
		/// 法線
		float normal[3];
		/// テクスチャ座標
		float uv[2];
		/// 追加テクスチャ座標
		float uva[4][4];
		/// スキニングタイプ
		PmxVertexSkinningType skinning_type;
		/// スキニング
		std::shared_ptr<PmxVertexSkinning> skinning;
		/// エッジ倍率
		float edge;
		void Read(std::istream *stream, PmxSetting *setting);
		void Write(std::ostream *stream, PmxSetting *setting);
	};

	/// マテリアル
	class PmxMaterial
	{
	public:
		PmxMaterial()
			: specularlity(0.0f)
			, flag(0)
			, edge_size(0.0f)
			, diffuse_texture_index(0)
			, sphere_texture_index(0)
			, sphere_op_mode(0)
			, common_toon_flag(0)
			, toon_texture_index(0)
			, index_count(0)
		{
			for (int i = 0; i < 3; ++i) {
				specular[i] = 0.0f;
				ambient[i] = 0.0f;
				edge_color[i] = 0.0f;
			}
			for (int i = 0; i < 4; ++i) {
				diffuse[i] = 0.0f;
			}
		}

		bool operator == (const PmxMaterial &v) const {
			return (std::memcmp(this, &v, sizeof(PmxMaterial)) == 0);
		}

		/// モデル名
		std::wstring material_name;
		/// モデル英名
		std::wstring material_english_name;
		/// 減衰色
		float diffuse[4];
		/// 光沢色
		float specular[3];
		/// 光沢度
		float specularlity;
		/// 環境色
		float ambient[3];
		/// 描画フラグ
		uint8_t flag;
		/// エッジ色
		float edge_color[4];
		/// エッジサイズ
		float edge_size;
		/// アルベドテクスチャインデックス
		int diffuse_texture_index;
		/// スフィアテクスチャインデックス
		int sphere_texture_index;
		/// スフィアテクスチャ演算モード
		uint8_t sphere_op_mode;
		/// 共有トゥーンフラグ
		uint8_t common_toon_flag;
		/// トゥーンテクスチャインデックス
		int toon_texture_index;
		/// メモ
		std::wstring memo;
		/// 頂点インデックス数
		int index_count;
		void Read(std::istream *stream, PmxSetting *setting);
		void Write(std::ostream *stream, PmxSetting *setting);
	};

	/// リンク
	class PmxIkLink
	{
	public:
		PmxIkLink()
			: link_target(0)
			, angle_lock(0)
		{
			for (int i = 0; i < 3; ++i) {
				max_radian[i] = 0.0f;
				min_radian[i] = 0.0f;
			}
		}

		bool operator == (const PmxIkLink &v) const {
			return (std::memcmp(this, &v, sizeof(PmxIkLink)) == 0);
		}
		/// リンクボーンインデックス
		int link_target;
		/// 角度制限
		uint8_t angle_lock;
		/// 最大制限角度
		float max_radian[3];
		/// 最小制限角度
		float min_radian[3];
		void Read(std::istream *stream, PmxSetting *settingn);
		void Write(std::ostream *stream, PmxSetting *settingn);
	};

	/// ボーン
	class PmxBone
	{
	public:
		PmxBone()
			: parent_index(0)
			, level(0)
			, bone_flag(0)
			, target_index(0)
			, grant_parent_index(0)
			, grant_weight(0.0f)
			, key(0)
			, ik_target_bone_index(0)
			, ik_loop(0)
			, ik_loop_angle_limit(0.0f)
			, ik_link_count(0)
		{
			for (int i = 0; i < 3; ++i) {
				position[i] = 0.0f;
				offset[i] = 0.0f;
				lock_axis_orientation[i] = 0.0f;
				local_axis_x_orientation[i] = 0.0f;
				local_axis_y_orientation[i] = 0.0f;
			}
		}

		bool operator == (const PmxBone &v) const {
			return (std::memcmp(this, &v, sizeof(PmxBone)) == 0);
		}

		/// ボーン名
		std::wstring bone_name;
		/// ボーン英名
		std::wstring bone_english_name;
		/// 位置
		float position[3];
		/// 親ボーンインデックス
		int parent_index;
		/// 階層
		int level;
		/// ボーンフラグ
		uint16_t bone_flag;
		/// 座標オフセット(has Target)
		float offset[3];
		/// 接続先ボーンインデックス(not has Target)
		int target_index;
		/// 付与親ボーンインデックス
		int grant_parent_index;
		/// 付与率
		float grant_weight;
		/// 固定軸の方向
		float lock_axis_orientation[3];
		/// ローカル軸のX軸方向
		float local_axis_x_orientation[3];
		/// ローカル軸のY軸方向
		float local_axis_y_orientation[3];
		/// 外部親変形のkey値
		int key;
		/// IKターゲットボーン
		int ik_target_bone_index;
		/// IKループ回数
		int ik_loop;
		/// IKループ計算時の角度制限(ラジアン)
		float ik_loop_angle_limit;
		/// IKリンク数
		int ik_link_count;
		/// IKリンク
		std::vector<PmxIkLink> ik_links;
		void Read(std::istream *stream, PmxSetting *setting);
		void Write(std::ostream *stream, PmxSetting *setting);
	};

	enum class MorphType : uint8_t
	{
		Group = 0,
		Vertex = 1,
		Bone = 2,
		UV = 3,
		AdditionalUV1 = 4,
		AdditionalUV2 = 5,
		AdditionalUV3 = 6,
		AdditionalUV4 = 7,
		Matrial = 8,
		Flip = 9,
		Implus = 10,
	};

	enum class MorphCategory : uint8_t
	{
		ReservedCategory = 0,
		Eyebrow = 1,
		Eye = 2,
		Mouth = 3,
		Other = 4,
	};

	class PmxMorphOffset
	{
	public:
		void virtual Read(std::istream *stream, PmxSetting *setting) = 0;
		void virtual Write(std::ostream *stream, PmxSetting *setting) = 0;
	};

	class PmxMorphVertexOffset : public PmxMorphOffset
	{
	public:
		PmxMorphVertexOffset()
			: vertex_index(0)
		{
			for (int i = 0; i < 3; ++i) {
				position_offset[i] = 0.0f;
			}
		}
		bool operator == (const PmxMorphVertexOffset &v) const {
			return (std::memcmp(this, &v, sizeof(PmxMorphVertexOffset)) == 0);
		}
		int vertex_index;
		float position_offset[3];
		void Read(std::istream *stream, PmxSetting *setting) override;
		void Write(std::ostream *stream, PmxSetting *setting) override;
	};

	class PmxMorphUVOffset : public PmxMorphOffset
	{
	public:
		PmxMorphUVOffset()
			: vertex_index(0)
		{
			for (int i = 0; i < 4; ++i) {
				uv_offset[i] = 0.0f;
			}
		}
		bool operator == (const PmxMorphUVOffset &v) const {
			return (std::memcmp(this, &v, sizeof(PmxMorphUVOffset)) == 0);
		}
		int vertex_index;
		float uv_offset[4];
		void Read(std::istream *stream, PmxSetting *setting) override;
		void Write(std::ostream *stream, PmxSetting *setting) override;
	};

	class PmxMorphBoneOffset : public PmxMorphOffset
	{
	public:
		PmxMorphBoneOffset()
			: bone_index(0)
		{
			for (int i = 0; i < 3; ++i) {
				translation[i] = 0.0f;
			}
			for (int i = 0; i < 4; ++i) {
				rotation[i] = 0.0f;
			}
		}
		bool operator == (const PmxMorphBoneOffset &v) const {
			return (std::memcmp(this, &v, sizeof(PmxMorphBoneOffset)) == 0);
		}
		int bone_index;
		float translation[3];
		float rotation[4];
		void Read(std::istream *stream, PmxSetting *setting) override;
		void Write(std::ostream *stream, PmxSetting *setting) override;
	};

	class PmxMorphMaterialOffset : public PmxMorphOffset
	{
	public:
		PmxMorphMaterialOffset()
			: specularity(0.0f)
			, edge_size(0.0f)
		{
			for (int i = 0; i < 3; ++i) {
				specular[i] = 0.0f;
				ambient[i] = 0.0f;
			}
			for (int i = 0; i < 4; ++i) {
				diffuse[i] = 0.0f;
				edge_color[i] = 0.0f;
				texture_argb[i] = 0.0f;
				sphere_texture_argb[i] = 0.0f;
				toon_texture_argb[i] = 0.0f;
			}
		}
		bool operator == (const PmxMorphMaterialOffset &v) const {
			return (std::memcmp(this, &v, sizeof(PmxMorphMaterialOffset)) == 0);
		}
		int material_index;
		uint8_t offset_operation;
		float diffuse[4];
		float specular[3];
		float specularity;
		float ambient[3];
		float edge_color[4];
		float edge_size;
		float texture_argb[4];
		float sphere_texture_argb[4];
		float toon_texture_argb[4];
		void Read(std::istream *stream, PmxSetting *setting) override;
		void Write(std::ostream *stream, PmxSetting *setting) override;
	};

	class PmxMorphGroupOffset : public PmxMorphOffset
	{
	public:
		PmxMorphGroupOffset()
			: morph_index(0)
			, morph_weight(0.0f)
		{}
		bool operator == (const PmxMorphGroupOffset &v) const {
			return (std::memcmp(this, &v, sizeof(PmxMorphGroupOffset)) == 0);
		}
		int morph_index;
		float morph_weight;
		void Read(std::istream *stream, PmxSetting *setting) override;
		void Write(std::ostream *stream, PmxSetting *setting) override;
	};

	class PmxMorphFlipOffset : public PmxMorphOffset
	{
	public:
		PmxMorphFlipOffset()
			: morph_index(0)
			, morph_value(0.0f)
		{}
		bool operator == (const PmxMorphFlipOffset &v) const {
			return (std::memcmp(this, &v, sizeof(PmxMorphFlipOffset)) == 0);
		}
		int morph_index;
		float morph_value;
		void Read(std::istream *stream, PmxSetting *setting) override;
		void Write(std::ostream *stream, PmxSetting *setting) override;
	};

	class PmxMorphImplusOffset : public PmxMorphOffset
	{
	public:
		PmxMorphImplusOffset()
			: rigid_body_index(0)
			, is_local(0)
		{
			for (int i = 0; i < 3; ++i) {
				velocity[i] = 0.0f;
				angular_torque[i] = 0.0f;
			}
		}
		bool operator == (const PmxMorphImplusOffset &v) const {
			return (std::memcmp(this, &v, sizeof(PmxMorphImplusOffset)) == 0);
		}

		int rigid_body_index;
		uint8_t is_local;
		float velocity[3];
		float angular_torque[3];
		void Read(std::istream *stream, PmxSetting *setting) override;
		void Write(std::ostream *stream, PmxSetting *setting) override;
	};

	/// モーフ
	class PmxMorph
	{
	public:
		PmxMorph()
			: offset_count(0)
		{
		}

		bool operator == (const PmxMorph &v) const {
			return (std::memcmp(this, &v, sizeof(PmxMorph)) == 0);
		}

		/// モーフ名
		std::wstring morph_name;
		/// モーフ英名
		std::wstring morph_english_name;
		/// カテゴリ
		MorphCategory category;
		/// モーフタイプ
		MorphType morph_type;
		/// オフセット数
		int offset_count;
		/// 頂点モーフ配列
		std::vector<PmxMorphVertexOffset> vertex_offsets;
		/// UVモーフ配列
		std::vector<PmxMorphUVOffset> uv_offsets;
		/// ボーンモーフ配列
		std::vector<PmxMorphBoneOffset> bone_offsets;
		/// マテリアルモーフ配列
		std::vector<PmxMorphMaterialOffset> material_offsets;
		/// グループモーフ配列
		std::vector<PmxMorphGroupOffset> group_offsets;
		/// フリップモーフ配列
		std::vector<PmxMorphFlipOffset> flip_offsets;
		/// インパルスモーフ配列
		std::vector<PmxMorphImplusOffset> implus_offsets;
		void Read(std::istream *stream, PmxSetting *setting);
		void Write(std::ostream *stream, PmxSetting *setting);
	};

	/// 枠内要素
	class PmxFrameElement
	{
	public:
		PmxFrameElement()
			: element_target(0)
			, index(0)
		{
		}

		bool operator == (const PmxFrameElement &v) const {
			return (std::memcmp(this, &v, sizeof(PmxFrameElement)) == 0);
		}

		/// 要素対象
		uint8_t element_target;
		/// 要素対象インデックス
		int index;
		void Read(std::istream *stream, PmxSetting *setting);
		void Write(std::ostream *stream, PmxSetting *setting);
	};

	/// 表示枠
	class PmxFrame
	{
	public:
		PmxFrame()
			: frame_flag(0)
			, element_count(0)
		{
		}

		bool operator == (const PmxFrame &v) const {
			return (std::memcmp(this, &v, sizeof(PmxFrame)) == 0);
		}

		/// 枠名
		std::wstring frame_name;
		/// 枠英名
		std::wstring frame_english_name;
		/// 特殊枠フラグ
		uint8_t frame_flag;
		/// 枠内要素数
		int element_count;
		/// 枠内要素配列
		std::vector<PmxFrameElement> elements;
		void Read(std::istream *stream, PmxSetting *setting);
		void Write(std::ostream *stream, PmxSetting *setting);
	};

	class PmxRigidBody
	{
	public:
		PmxRigidBody()
			: target_bone(0)
			, group(0)
			, mask(0)
			, shape(0)
			, mass(0.0f)
			, move_attenuation(0.0f)
			, rotation_attenuation(0.0f)
			, repulsion(0.0f)
			, friction(0.0f)
			, physics_calc_type(0)
		{
			for (int i = 0; i < 3; ++i) {
				size[i] = 0.0f;
				position[i] = 0.0f;
				orientation[i] = 0.0f;
			}
		}

		bool operator == (const PmxRigidBody &v) const {
			return (std::memcmp(this, &v, sizeof(PmxRigidBody)) == 0);
		}

		/// 剛体名
		std::wstring rigid_body_name;
		/// 剛体英名
		std::wstring rigid_body_english_name;
		/// 関連ボーンインデックス
		int target_bone;
		/// グループ
		uint8_t group;
		/// マスク
		uint16_t mask;
		/// 形状
		uint8_t shape;
		float size[3];
		float position[3];
		float orientation[3];
		float mass;
		float move_attenuation;
		float rotation_attenuation;
		float repulsion;
		float friction;
		uint8_t physics_calc_type;
		void Read(std::istream *stream, PmxSetting *setting);
		void Write(std::ostream *stream, PmxSetting *setting);
	};

	enum class PmxJointType : uint8_t
	{
		Generic6DofSpring = 0,
		Generic6Dof = 1,
		Point2Point = 2,
		ConeTwist = 3,
		Slider = 5,
		Hinge = 6
	};

	class PmxJointParam
	{
	public:
		PmxJointParam()
			: rigid_body1(0)
			, rigid_body2(0)
		{
			for (int i = 0; i < 3; ++i) {
				position[i] = 0.0f;
				orientaiton[i] = 0.0f;
				move_limitation_min[i] = 0.0f;
				move_limitation_max[i] = 0.0f;
				rotation_limitation_min[i] = 0.0f;
				rotation_limitation_max[i] = 0.0f;
				spring_move_coefficient[i] = 0.0f;
				spring_rotation_coefficient[i] = 0.0f;
			}
		}
		int rigid_body1;
		int rigid_body2;
		float position[3];
		float orientaiton[3];
		float move_limitation_min[3];
		float move_limitation_max[3];
		float rotation_limitation_min[3];
		float rotation_limitation_max[3];
		float spring_move_coefficient[3];
		float spring_rotation_coefficient[3];
		void Read(std::istream *stream, PmxSetting *setting);
		void Write(std::ostream *stream, PmxSetting *setting);
	};

	class PmxJoint
	{
	public:
		std::wstring joint_name;
		std::wstring joint_english_name;
		PmxJointType joint_type;
		PmxJointParam param;
		bool operator == (const PmxJoint &v) const {
			return (std::memcmp(this, &v, sizeof(PmxJoint)) == 0);
		}
		void Read(std::istream *stream, PmxSetting *setting);
		void Write(std::ostream *stream, PmxSetting *setting);
	};

	enum PmxSoftBodyFlag : uint8_t
	{
		BLink = 0x01,
		Cluster = 0x02,
		Link = 0x04
	};

	class PmxAncherRigidBody
	{
	public:
		PmxAncherRigidBody()
			: related_rigid_body(0)
			, related_vertex(0)
			, is_near(false)
		{}
		int related_rigid_body;
		int related_vertex;
		bool is_near;
		void Read(std::istream *stream, PmxSetting *setting);
		void Write(std::ostream *stream, PmxSetting *setting);
	};

	class PmxSoftBody
	{
	public:
		PmxSoftBody()
			: shape(0)
			, target_material(0)
			, group(0)
			, mask(0)
			, blink_distance(0)
			, cluster_count(0)
			, mass(0.0)
			, collisioni_margin(0.0)
			, aero_model(0)
			, VCF(0.0f)
			, DP(0.0f)
			, DG(0.0f)
			, LF(0.0f)
			, PR(0.0f)
			, VC(0.0f)
			, DF(0.0f)
			, MT(0.0f)
			, CHR(0.0f)
			, KHR(0.0f)
			, SHR(0.0f)
			, AHR(0.0f)
			, SRHR_CL(0.0f)
			, SKHR_CL(0.0f)
			, SSHR_CL(0.0f)
			, SR_SPLT_CL(0.0f)
			, SK_SPLT_CL(0.0f)
			, SS_SPLT_CL(0.0f)
			, V_IT(0)
			, P_IT(0)
			, D_IT(0)
			, C_IT(0)
			, LST(0.0f)
			, AST(0.0f)
			, VST(0.0f)
			, anchor_count(0)
			, pin_vertex_count(0)
		{}

		bool operator == (const PmxSoftBody &v) const {
			return (std::memcmp(this, &v, sizeof(PmxSoftBody)) == 0);
		}

		std::wstring soft_body_name;
		std::wstring soft_body_english_name;
		uint8_t shape;
		int target_material;
		uint8_t group;
		uint16_t mask;
		PmxSoftBodyFlag flag;
		int blink_distance;
		int cluster_count;
		float mass;
		float collisioni_margin;
		int aero_model;
		float VCF;
		float DP;
		float DG;
		float LF;
		float PR;
		float VC;
		float DF;
		float MT;
		float CHR;
		float KHR;
		float SHR;
		float AHR;
		float SRHR_CL;
		float SKHR_CL;
		float SSHR_CL;
		float SR_SPLT_CL;
		float SK_SPLT_CL;
		float SS_SPLT_CL;
		int V_IT;
		int P_IT;
		int D_IT;
		int C_IT;
		float LST;
		float AST;
		float VST;
		int anchor_count;
		std::vector<PmxAncherRigidBody> anchers;
		int pin_vertex_count;
		std::vector<int> pin_vertices;
		void Read(std::istream *stream, PmxSetting *setting);
		void Write(std::ostream *stream, PmxSetting *setting);
	};

	/// PMXモデル
	class PmxModel
	{
	public:
		PmxModel()
			: version(0.0f)
			, vertex_count(0)
			, index_count(0)
			, texture_count(0)
			, material_count(0)
			, bone_count(0)
			, morph_count(0)
			, frame_count(0)
			, rigid_body_count(0)
			, joint_count(0)
			, soft_body_count(0)
		{}

		/// バージョン
		float version;
		/// 設定
		PmxSetting setting;
		/// モデル名
		std::wstring model_name;
		/// モデル英名
		std::wstring model_english_name;
		/// コメント
		std::wstring model_comment;
		/// 英語コメント
		std::wstring model_english_commnet;
		/// 頂点数
		int vertex_count;
		/// 頂点配列
		std::vector<PmxVertex> vertices;
		/// インデックス数
		int index_count;
		/// インデックス配列
		std::vector<int> indices;
		/// テクスチャ数
		int texture_count;
		/// テクスチャ配列
		std::vector<std::wstring> textures;
		/// マテリアル数
		int material_count;
		/// マテリアル
		std::vector<PmxMaterial> materials;
		/// ボーン数
		int bone_count;
		/// ボーン配列
		std::vector<PmxBone> bones;
		/// モーフ数
		int morph_count;
		/// モーフ配列
		std::vector<PmxMorph> morphs;
		/// 表示枠数
		int frame_count;
		/// 表示枠配列
		std::vector<PmxFrame> frames;
		/// 剛体数
		int rigid_body_count;
		/// 剛体配列
		std::vector<PmxRigidBody> rigid_bodies;
		/// ジョイント数
		int joint_count;
		/// ジョイント配列
		std::vector<PmxJoint> joints;
		/// ソフトボディ数
		int soft_body_count;
		/// ソフトボディ配列
		std::vector<PmxSoftBody> soft_bodies;
		/// モデル初期化
		void Init();
		/// モデル読み込み
		void Read(std::istream *stream);
		/// モデル書き込み
		void Write(std::ostream *stream);
		///// ファイルからモデルの読み込み
		//static std::unique_ptr<PmxModel> ReadFromFile(const char *filename);
		///// 入力ストリームからモデルの読み込み
		//static std::unique_ptr<PmxModel> ReadFromStream(std::istream *stream);
	};
}