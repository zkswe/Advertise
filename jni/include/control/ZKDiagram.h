/*
 * ZKDiagram.h
 *
 *  Created on: Jul 31, 2017
 *      Author: guoxs
 */

#ifndef _CONTROL_ZKDIAGRAM_H_
#define _CONTROL_ZKDIAGRAM_H_

#include <vector>
#include "ZKBase.h"

class ZKDiagram : public ZKBase {
public:
	typedef enum {
		E_DIAGRAM_STYLE_LINE,		// 折线
		E_DIAGRAM_STYLE_CURVE		// 曲线
	} EDiagramStyle;

public:
	ZKDiagram(HWND hParentWnd);
	virtual ~ZKDiagram();

	void setPenWidth(int index, int width);
	void setPenColor(int index, ARGB color);
	void setXScale(int index, double xScale);
	void setYScale(int index, double yScale);
	void setData(int index, const MPPOINT *pPoints, int count);

	void addDiagramInfo(int width, ARGB color, int style, double xScale = 1.0, double yScale = 1.0);

protected:
	virtual void onBeforeCreateWindow(const Json::Value &json);
	virtual const char* getClassName() const { return ZK_DIAGRAM; }

	virtual void onDraw(HDC hdc);

	void _section_(zk) drawDiagram(HDC hdc);

private:
	void _section_(zk) parseDiagramAttributeFromJson(const Json::Value &json);

private:
	typedef struct {
		float lower;
		float upper;
	} SAxisRange;

	typedef struct {
		HPATH path;
		HPEN pen;
		int penWidth;
		ARGB penColor;
		int style;
		double xScale;
		double yScale;
	} SDiagramInfo;

	void setTransform(SDiagramInfo &diagramInfo);

private:
	vector<SDiagramInfo *> mDiagramInfoList;

	HGRAPHICS mBackgroundGraphics;

	LayoutPosition mRegionPosition;		// 曲线图绘制区域

	SAxisRange mXAxisRange;		// X轴坐标范围
	SAxisRange mYAxisRange;		// Y轴坐标范围

	double mXBaseScale;
	double mYBaseScale;

	bool mIsAntialiasOn;		// 是否打开抗锯齿
};

#endif /* _CONTROL_ZKDIAGRAM_H_ */
