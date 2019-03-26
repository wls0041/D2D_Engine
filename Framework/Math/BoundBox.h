#pragma once

class BoundBox
{
public:
	BoundBox();
	BoundBox(const Vector3& minBox, const Vector3& maxBox);
	BoundBox(const std::vector<struct VertexTexture>& vertices);
	virtual ~BoundBox();

	const Vector3& GetMin() const { return minBox; }
	const Vector3& GetMax() const { return maxBox; }
	const Vector3 GetCenter() const { return (maxBox + minBox) * 0.5f; }
	const Vector3 GetSize() const { return maxBox - minBox; }
	const Vector3 GetExtents() const { return (maxBox - minBox) * 0.5f; }

	Intersection IsInside(const Vector3& point);
	Intersection IsInside(const BoundBox& box);

	const BoundBox Transformed(const Matrix& matrix);
	BoundBox &operator=(const BoundBox &rhs) {
		minBox = rhs.minBox;
		maxBox = rhs.maxBox;
		return *this;
	}

	const bool operator==(const BoundBox& rhs) const
	{
		bool bCheck = true;
		bCheck &= (minBox == rhs.minBox);
		bCheck &= (maxBox == rhs.maxBox);
		return bCheck;
	}

private:
	Vector3 minBox;
	Vector3 maxBox;
};