const fs = require('fs');
const docx = require('docx');
const { Document, Packer, Paragraph, TextRun, ImageRun, Table, TableRow, TableCell, WidthType } = docx;

async function createDocument() {
    const doc = new Document({
        sections: [
            {
                properties: {},
                children: [
                    // Title Section
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "COMSATS University Islamabad, Lahore Campus",
                                bold: true,
                                size: 24,
                            }),
                        ],
                        alignment: 'center',
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "Department of Electrical and Computer Engineering",
                                size: 24,
                            }),
                        ],
                        alignment: 'center',
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "CSC462 - Artificial Intelligence",
                                size: 24,
                            }),
                        ],
                        alignment: 'center',
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "Assignment 1",
                                size: 24,
                            }),
                        ],
                        alignment: 'center',
                    }),
                    // Student Info
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "Name: Abdullah Laeeq",
                                size: 20,
                            }),
                        ],
                        alignment: 'left',
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "Reg No: FA22-BCE-026",
                                size: 20,
                            }),
                        ],
                        alignment: 'left',
                    }),
                    // Pseudocode Section
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "Pseudocode for Simple Linear Regression",
                                bold: true,
                                size: 20,
                            }),
                        ],
                        heading: 'Heading1',
                        spacing: { before: 400 },
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "Simple Linear Regression Learning Algorithm:",
                                bold: true,
                            }),
                        ],
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "Input: Training data X_train (array of Years of Experience), y_train (array of Salary)",
                            }),
                        ],
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "Output: Coefficients b0 (intercept), b1 (slope)",
                            }),
                        ],
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "1. Calculate n = length of X_train",
                            }),
                        ],
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "2. Calculate sum_x = sum of all elements in X_train",
                            }),
                        ],
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "3. Calculate sum_y = sum of all elements in y_train",
                            }),
                        ],
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "4. Calculate sum_xy = sum of (X_train[i] * y_train[i]) for i from 0 to n-1",
                            }),
                        ],
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "5. Calculate sum_x2 = sum of (X_train[i]^2) for i from 0 to n-1",
                            }),
                        ],
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "6. Calculate b1 = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x^2)",
                            }),
                        ],
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "7. Calculate b0 = (sum_y - b1 * sum_x) / n",
                            }),
                        ],
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "8. Return b0, b1",
                            }),
                        ],
                    }),
                    // Manual Implementation Scatter Plot
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "Scatter Plot and Regression Line (Manual Implementation)",
                                bold: true,
                                size: 20,
                            }),
                        ],
                        heading: 'Heading1',
                        spacing: { before: 400 },
                    }),
                    new Paragraph({
                        children: [
                            new ImageRun({
                                data: fs.readFileSync('manual_regression.png'),
                                transformation: {
                                    width: 500,
                                    height: 300,
                                },
                            }),
                        ],
                        alignment: 'center',
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "Figure 1: Scatter Plot and Regression Line from Manual Linear Regression",
                                italics: true,
                            }),
                        ],
                        alignment: 'center',
                        spacing: { after: 200 },
                    }),
                    // sklearn Implementation Scatter Plot
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "Scatter Plot and Regression Line (sklearn Implementation)",
                                bold: true,
                                size: 20,
                            }),
                        ],
                        heading: 'Heading1',
                        spacing: { before: 400 },
                    }),
                    new Paragraph({
                        children: [
                            new ImageRun({
                                data: fs.readFileSync('sklearn_regression.png'),
                                transformation: {
                                    width: 500,
                                    height: 300,
                                },
                            }),
                        ],
                        alignment: 'center',
                    }),
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "Figure 2: Scatter Plot and Regression Line from sklearn Linear Regression",
                                italics: true,
                            }),
                        ],
                        alignment: 'center',
                        spacing: { after: 200 },
                    }),
                    // Table of Coefficients
                    new Paragraph({
                        children: [
                            new TextRun({
                                text: "Table of Coefficients",
                                bold: true,
                                size: 20,
                            }),
                        ],
                        heading: 'Heading1',
                        spacing: { before: 400 },
                    }),
                    new Table({
                        rows: [
                            new TableRow({
                                children: [
                                    new TableCell({
                                        children: [new Paragraph("Model")],
                                        width: { size: 33, type: WidthType.PERCENTAGE },
                                    }),
                                    new TableCell({
                                        children: [new Paragraph("Intercept")],
                                        width: { size: 33, type: WidthType.PERCENTAGE },
                                    }),
                                    new TableCell({
                                        children: [new Paragraph("Slope")],
                                        width: { size: 33, type: WidthType.PERCENTAGE },
                                    }),
                                ],
                            }),
                            new TableRow({
                                children: [
                                    new TableCell({
                                        children: [new Paragraph("Manual Implementation")],
                                    }),
                                    new TableCell({
                                        children: [new Paragraph("25202.89")], // Replace with actual value
                                    }),
                                    new TableCell({
                                        children: [new Paragraph("9731.20")],  // Replace with actual value
                                    }),
                                ],
                            }),
                            new TableRow({
                                children: [
                                    new TableCell({
                                        children: [new Paragraph("sklearn Implementation")],
                                    }),
                                    new TableCell({
                                        children: [new Paragraph("25202.89")], // Replace with actual value
                                    }),
                                    new TableCell({
                                        children: [new Paragraph("9731.20")],  // Replace with actual value
                                    }),
                                ],
                            }),
                        ],
                        width: { size: 100, type: WidthType.PERCENTAGE },
                    }),
                ],
            },
        ],
    });

    const buffer = await Packer.toBuffer(doc);
    fs.writeFileSync('Assignment1.docx', buffer);
    console.log('Document created successfully');
}

createDocument().catch(err => console.error('Error creating document:', err));