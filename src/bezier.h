void quadratic_bezier_curve(float px0, float py0, float px1, float py1, float px2, float py2, float t, float &pxo, float &pyo)
{
    pxo = pow(1 - t, 2) * px0 + (1 - t) * 2 * t * px1 + t * t * px2;
    pyo = pow(1 - t, 2) * py0 + (1 - t) * 2 * t * py1 + t * t * py2;
}

void cubic_bezier_curve(float px0, float py0, float px1, float py1, float px2, float py2, float px3, float py3, float t, float &pxo, float &pyo)
{
    pxo = pow(1 - t, 3) * px0 + pow(1 - t, 2) * 3 * t * px1 + (1 - t) * 3 * t * t * px2 + t * t * t * px3;
    pyo = pow(1 - t, 3) * py0 + pow(1 - t, 2) * 3 * t * py1 + (1 - t) * 3 * t * t * py2 + t * t * t * py3;
}