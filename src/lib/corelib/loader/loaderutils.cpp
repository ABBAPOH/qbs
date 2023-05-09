/****************************************************************************
**
** Copyright (C) 2023 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qbs.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "loaderutils.h"

#include "productitemmultiplexer.h"

#include <language/evaluator.h>
#include <language/item.h>
#include <language/language.h>
#include <language/value.h>
#include <logging/categories.h>
#include <logging/translator.h>
#include <tools/stringconstants.h>

namespace qbs::Internal {

void mergeParameters(QVariantMap &dst, const QVariantMap &src)
{
    for (auto it = src.begin(); it != src.end(); ++it) {
        if (it.value().userType() == QMetaType::QVariantMap) {
            QVariant &vdst = dst[it.key()];
            QVariantMap mdst = vdst.toMap();
            mergeParameters(mdst, it.value().toMap());
            vdst = mdst;
        } else {
            dst[it.key()] = it.value();
        }
    }
}

ShadowProductInfo getShadowProductInfo(const ProductContext &product)
{
    const bool isShadowProduct = product.name.startsWith(StringConstants::shadowProductPrefix());
    return std::make_pair(isShadowProduct,
                          isShadowProduct
                              ? product.name.mid(StringConstants::shadowProductPrefix().size())
                              : QString());
}

void adjustParametersScopes(Item *item, Item *scope)
{
    if (item->type() == ItemType::ModuleParameters) {
        item->setScope(scope);
        return;
    }

    for (const auto &value : item->properties()) {
        if (value->type() == Value::ItemValueType)
            adjustParametersScopes(std::static_pointer_cast<ItemValue>(value)->item(), scope);
    }
}

QString ProductContext::uniqueName() const
{
    return ResolvedProduct::uniqueName(name, multiplexConfigurationId);
}

QString ProductContext::displayName() const
{
    return ProductItemMultiplexer::fullProductDisplayName(name, multiplexConfigurationId);
}

void ProductContext::handleError(const ErrorInfo &error)
{
    const bool alreadyHadError = info.delayedError.hasError();
    if (!alreadyHadError) {
        info.delayedError.append(Tr::tr("Error while handling product '%1':")
                                     .arg(name), item->location());
    }
    if (error.isInternalError()) {
        if (alreadyHadError) {
            qCDebug(lcModuleLoader()) << "ignoring subsequent internal error" << error.toString()
                                      << "in product" << name;
            return;
        }
    }
    const auto errorItems = error.items();
    for (const ErrorItem &ei : errorItems)
        info.delayedError.append(ei.description(), ei.codeLocation());
    project->topLevelProject->productInfos[item] = info;
    project->topLevelProject->disabledItems << item;
    project->topLevelProject->erroneousProducts.insert(name);
}

bool TopLevelProjectContext::checkItemCondition(Item *item, Evaluator &evaluator)
{
    if (evaluator.boolValue(item, StringConstants::conditionProperty()))
        return true;
    disabledItems += item;
    return false;
}

} // namespace qbs::Internal
